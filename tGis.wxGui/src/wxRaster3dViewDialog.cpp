#include "wxRaster3dViewDialog.h"
#include "wxTGisApplication.h"
#include "wxGradientColorPickerDialog.h"

BEGIN_NAME_SPACE(tGis, Gui)

//进行三维显示最多允许的像素数量 2000*1000
//如果所选范围超过2000*1000个像素，则需要重采样
#define MAX_PIXEL 2000000


static inline void gluLookAt_(GLfloat eyex, GLfloat eyey, GLfloat eyez,
	GLfloat centerx, GLfloat centery, GLfloat centerz,
	GLfloat upx, GLfloat upy, GLfloat upz)

{
	GLfloat m[16];
	GLfloat x[3], y[3], z[3];
	GLfloat mag;

	/* Make rotation matrix */

	/* Z vector */
	z[0] = eyex - centerx;
	z[1] = eyey - centery;
	z[2] = eyez - centerz;

	mag = (GLfloat)sqrt(z[0] * z[0] + z[1] * z[1] + z[2] * z[2]);

	if (mag) {                   /* mpichler, 19950515 */
		z[0] /= mag;
		z[1] /= mag;
		z[2] /= mag;
	}

	/* Y vector */
	y[0] = upx;
	y[1] = upy;
	y[2] = upz;

	/* X vector = Y cross Z */
	x[0] = y[1] * z[2] - y[2] * z[1];
	x[1] = -y[0] * z[2] + y[2] * z[0];
	x[2] = y[0] * z[1] - y[1] * z[0];

	/* Recompute Y = Z cross X */
	y[0] = z[1] * x[2] - z[2] * x[1];
	y[1] = -z[0] * x[2] + z[2] * x[0];
	y[2] = z[0] * x[1] - z[1] * x[0];


	/* mpichler, 19950515 */

	/* cross product gives area of parallelogram, which is < 1.0 for
	* non-perpendicular unit-length vectors; so normalize x, y here
	*/

	mag = sqrt(x[0] * x[0] + x[1] * x[1] + x[2] * x[2]);

	if (mag) {
		x[0] /= mag;
		x[1] /= mag;
		x[2] /= mag;
	}

	mag = sqrt(y[0] * y[0] + y[1] * y[1] + y[2] * y[2]);

	if (mag) {
		y[0] /= mag;
		y[1] /= mag;
		y[2] /= mag;
	}

#define M(row,col)  m[col*4+row]
	M(0, 0) = x[0];
	M(0, 1) = x[1];
	M(0, 2) = x[2];
	M(0, 3) = 0.0;
	M(1, 0) = y[0];
	M(1, 1) = y[1];
	M(1, 2) = y[2];
	M(1, 3) = 0.0;
	M(2, 0) = z[0];
	M(2, 1) = z[1];
	M(2, 2) = z[2];
	M(2, 3) = 0.0;
	M(3, 0) = 0.0;
	M(3, 1) = 0.0;
	M(3, 2) = 0.0;
	M(3, 3) = 1.0;
#undef M
	glMultMatrixf(m);

	/* Translate Eye to Origin */
	glTranslatef(-eyex, -eyey, -eyez);
}

static inline void gluPerspective_(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	double xmin, xmax, ymin, ymax;

	ymax = zNear * tan(fovy * 3.14159265 / 360.0);
	ymin = -ymax;
	xmin = ymin * aspect;
	xmax = ymax * aspect;

	glFrustum(xmin, xmax, ymin, ymax, zNear, zFar);
}

static inline void CalcTriNormal(float *vertex1, float *vertex2, float *vertex3, float *normal)
{
	//计算三角形的法向量
	float vector1[3], vector2[3];

	vector1[0] = vertex2[0] - vertex1[0];//X
	vector1[1] = vertex2[1] - vertex1[1];//Y
	vector1[2] = vertex2[2] - vertex1[2];//Z

	vector2[0] = vertex3[0] - vertex1[0];//X
	vector2[1] = vertex3[1] - vertex1[1];//Y
	vector2[2] = vertex3[2] - vertex1[2];//Z

										 // 求向量vector1和vector2叉乘
	normal[0] = vector1[1] * vector2[2] - vector1[2] * vector2[1];
	normal[1] = vector1[2] * vector2[0] - vector1[0] * vector2[2];
	normal[2] = vector1[0] * vector2[1] - vector1[1] * vector2[0];

	float normalLength;
	normalLength = (float)sqrt(normal[0] * normal[0] + normal[1] * normal[1] + normal[2] * normal[2]);
	if (normalLength != 0.0)
	{
		normal[0] /= normalLength;
		normal[1] /= normalLength;
		normal[2] /= normalLength;
	}
}

class wxRaster3dViewDialogImpl : public wxGLCanvas
{
public:
	wxRaster3dViewDialogImpl(wxRaster3dViewDialog* owner,
		wxWindow *parent,
		wxWindowID id = wxID_ANY,
		const int *attribList = NULL,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = 0,
		const wxString& name = wxGLCanvasName,
		const wxPalette& palette = wxNullPalette)
		: wxGLCanvas(parent, id, attribList, pos, size, style, name, palette)
		, _glContext(this)
	{
		_owner = owner;

		_color = new GradientColor();
		_color->AddKeyColor(0, 1, 128, 0);
		_color->AddKeyColor(13, 255, 233, 1);
		_color->AddKeyColor(223, 213, 32, 2);
		_color->AddKeyColor(196, 21, 0, 3);

		_bandIndex = 0;
		_raster = nullptr;
		_pixBuffer = (float*)malloc(MAX_PIXEL * sizeof(float));
		//顶点颜色，*4而不是*3为了尽量内存对齐
		_vertexColor = (unsigned char*)malloc(MAX_PIXEL * 4);
		//一个矩形分成两个三角形 最多有(w-1)*(h-1)*2个三角形
		_triNormal = (float*)malloc(6 * MAX_PIXEL * sizeof(float));
		_pitchY = 0.0f;
		_rollX = -35.0f;
		_yawZ = 0.0f;
		_scaleZ = 1.0f;
		_seeX = 0.0f;
		_seeY = 0.0f;

		_gradColorFill = true;
		_showRefLevel = true;
		_showBoundary = true;

		_boundaryMask = (unsigned char*)malloc(MAX_PIXEL);

		_glContext.SetCurrent(*this);
		glClearDepth(1.0f);
		glEnable(GL_DEPTH_TEST);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		GLfloat m_light0Ambient[4] = { 0.6f,0.6f,0.6f,0.8f };
		GLfloat m_light0Diffuse[4] = { 0.6f,0.7f,0.6f,0.6f };
		GLfloat m_light0Pos[4] = { -1.0f,-1.0f,-1.0f,0.0f };
		GLfloat m_light1Pos[4] = { 1.0f,1.0f,1.0f,0.0f };

		glLightfv(GL_LIGHT0, GL_AMBIENT, m_light0Ambient);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, m_light0Diffuse);
		glLightfv(GL_LIGHT0, GL_POSITION, m_light0Pos);
		glLightfv(GL_LIGHT1, GL_AMBIENT, m_light0Ambient);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, m_light0Diffuse);
		glLightfv(GL_LIGHT1, GL_POSITION, m_light1Pos);
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
	}

	~wxRaster3dViewDialogImpl()
	{
		_color->Release();
		free(_pixBuffer);
		free(_triNormal);
		free(_vertexColor);
		free(_boundaryMask);
	}

public:
	GradientColor* _color;
	wxGLContext _glContext;
	wxRaster3dViewDialog* _owner;
	MyGDALRasterDataset * _raster;
	int _bandIndex;
	IVertex2dList* _polygon;

	int _pixLeft;
	int _pixTop;
	int _pixWidth;
	int _pixHeight;
	int _bufWidth;
	int _bufHeight;	
	float* _pixBuffer;
	float _bufStepX;
	float _bufStepY;
	float _bufPixArea;
	//栅格数据左上角的空间坐标值
	float _startX;
	float _startY;
	//栅格数据空间范围的一半
	float _halfRangeX;
	float _halfRangeY;
	//栅格数据像素最小最大值，就是Z值
	float _minPixValue;
	float _maxPixValue;
	//存储三角形单位法向量
	float* _triNormal;
	//用于填充顶点的颜色
	unsigned char* _vertexColor;

	//目视点位置(_seeX,_seeY,0)，盯着看的位置
	float _seeX;
	float _seeY;
	//观察者位置(0,0,_eyeZ)，眼睛位置
	float _eyeZ;
	//欧拉角旋转
	float _yawZ;
	float _rollX;
	float _pitchY;
	//z值缩放比例
	float _scaleZ;

	//参考Z值平面
	float _refLevel;
	float _volAbove;
	float _volBelow;

	//是否用根据Z值用渐变色填充
	bool _gradColorFill;
	//是否显示参考Z值平面
	bool _showRefLevel;
	//是否显示范围框
	bool _showBoundary;

	//范围框掩码，范围框内大于0，范围框外等于0
	unsigned char* _boundaryMask;

public:
	void SetRaster(MyGDALRasterDataset* raster, IVertex2dList* polygon)
	{
		_raster = raster;
		_polygon = polygon;
		double minX = INT_MAX;
		double maxX = INT_MIN;
		double minY = INT_MAX;
		double maxY = INT_MIN;
		size_t vertexCount = polygon->GetVertexCount();
		for (size_t i = 0; i < vertexCount; i++)
		{
			double x, y;
			polygon->GetVertex(i, &x, &y);
			if (minX > x)
				minX = x;
			if (maxX < x)
				maxX = x;
			if (minY > y)
				minY = y;
			if (maxY < y)
				maxY = y;
		}

		double cenX = (minX + maxX) / 2.0;
		double cenY = (minY + maxY) / 2.0;
		double halfW = (maxX - minX)*0.75;
		double halfH = (maxY - minY)*0.75;

		minX = cenX - halfW;
		maxX = cenX + halfW;
		minY = cenY - halfH;
		maxY = cenY + halfH;

		double left, top, right, bottom;
		raster->Spatial2Pixel(minX, maxY, &left, &top);
		raster->Spatial2Pixel(maxX, minY, &right, &bottom);

		int xSize = raster->GetGDALDataset()->GetRasterXSize();
		int ySize = raster->GetGDALDataset()->GetRasterYSize();

		if (left > right)
		{
			double temp = left;
			left = right;
			right = temp;
		}

		if (left < 0)
			_pixLeft = 0;
		else
			_pixLeft = (int)round(left);

		if (right > xSize)
			_pixWidth = xSize - _pixLeft;
		else
			_pixWidth = (int)round(right) - _pixLeft;

		if (bottom < top)
		{
			double temp = bottom;
			bottom = top;
			top = temp;
		}

		if (top < 0)
			_pixTop = 0;
		else
			_pixTop = (int)round(top);

		if (bottom > ySize)
			_pixHeight = ySize - _pixTop;
		else
			_pixHeight = (int)round(bottom) - _pixTop;

		double pixCount = _pixWidth*_pixHeight;
		if (pixCount < MAX_PIXEL)
		{
			_bufWidth = _pixWidth;
			_bufHeight = _pixHeight;
		}
		else
		{
			double ratio = sqrt(MAX_PIXEL / pixCount);
			_bufWidth = int(_pixWidth*ratio);
			_bufHeight = int(_pixHeight*ratio);
		}

		_raster->Pixel2Spatial(_pixLeft, _pixTop, &minX, &minY);
		_raster->Pixel2Spatial(_pixLeft + _pixWidth, _pixTop + _pixHeight, &maxX, &maxY);

		double rangeX = maxX - minX;
		double rangeY = maxY - minY;

		_startX = minX;
		_startY = minY;
		_halfRangeX = rangeX / 2.0;
		_halfRangeY = rangeY / 2.0;
		_bufStepX = rangeX / _bufWidth;
		_bufStepY = rangeY / _bufHeight;
		_bufPixArea = abs(_bufStepX*_bufStepY);

		POINT_2I_T* pts = (POINT_2I_T*)malloc(vertexCount * sizeof(POINT_2I_T));
		POINT_2I_T* pt = pts;
		for (size_t i = 0; i < vertexCount; i++)
		{
			double x, y;
			polygon->GetVertex(i, &x, &y);
			x = x - _startX;
			y = y - _startY;
			pt->x = (int)round(x / _bufStepX);
			pt->y = (int)round(y / _bufStepY);
			pt++;
		}

		memset(_boundaryMask, 0, MAX_PIXEL);
		RasterizePolygon(pts, vertexCount, SetBoundaryMask, this);
		//WriteMemoryBlock("E:\\tst.tif", (void**)(&_boundaryMask), GDT_Byte, _bufWidth, _bufHeight);
		free(pts);
	}

	static void __stdcall SetBoundaryMask(void* user, int x, int y)
	{
		wxRaster3dViewDialogImpl* impl = (wxRaster3dViewDialogImpl*)user;
		*(impl->_boundaryMask + y*impl->_bufWidth + x) = 255;
	}

	void InitData(int bandIndex)
	{
		_bandIndex = bandIndex;
		GDALRasterBand* band = _raster->GetGDALDataset()->GetRasterBand(_bandIndex);

		GDALRasterIOExtraArg arg;
		INIT_RASTERIO_EXTRA_ARG(arg);
		arg.eResampleAlg = GRIORA_NearestNeighbour;
		band->RasterIO(GF_Read, _pixLeft, _pixTop, _pixWidth, _pixHeight, _pixBuffer, _bufWidth, _bufHeight, GDT_Float32, 0, 0, &arg);
		_minPixValue = FLT_MAX;
		_maxPixValue = -FLT_MAX;

		int noDataOk = 0;
		double noDataValue = band->GetNoDataValue(&noDataOk);
		float* pixLine1 = _pixBuffer;
		float* pixLine2 = _pixBuffer + _pixWidth;

		float x = 0;
		float y = 0;
		float* triNormalValue = _triNormal;
		float vertex1[3];
		float vertex2[3];
		float vertex3[3];
		float vertex4[3];

		for (int i = 0; i < _bufHeight - 1; i++)
		{
			for (int j = 0; j < _bufWidth - 1; j++)
			{
				vertex1[2] = *pixLine1;
				vertex2[2] = *(pixLine1 + 1);
				vertex3[2] = *(pixLine2 + 1);
				vertex4[2] = *pixLine2;

#define _TEST_AND_SET(v,pixValue)\
			    if (noDataOk != 0 && v == noDataValue)\
			    {\
			    	*(pixValue) = 0;\
                    v = 0;\
			    }\
			    else\
			    {\
			    	if (_minPixValue > v)\
			    		_minPixValue = v;\
			    	if (_maxPixValue < v)\
			    		_maxPixValue = v;\
			    }
				_TEST_AND_SET(vertex1[2], pixLine1);
				_TEST_AND_SET(vertex2[2], pixLine1 + 1);
				_TEST_AND_SET(vertex3[2], pixLine1 + 1);
				_TEST_AND_SET(vertex4[2], pixLine2);

#undef _TEST_AND_SET

				vertex1[0] = x;
				vertex2[0] = x + _bufStepX;
				vertex3[0] = x + _bufStepX;
				vertex4[0] = x;

				vertex1[1] = y;
				vertex2[1] = y;
				vertex3[1] = y + _bufStepY;
				vertex4[1] = y + _bufStepY;

				CalcTriNormal(vertex1, vertex2, vertex4, triNormalValue);
				triNormalValue += 3;
				CalcTriNormal(vertex2, vertex3, vertex4, triNormalValue);
				triNormalValue += 3;

				pixLine1++;
				pixLine2++;

				x += _bufStepX;
			}

			pixLine1++;
			pixLine2++;
			y += _bufStepY;
			x = 0;
		}

		double pixValueRange = _maxPixValue - _minPixValue;

		if (pixValueRange <= DBL_EPSILON)
		{
			_minPixValue = 0.0f;
			pixValueRange = _maxPixValue;
		}

		float* pixValue = _pixBuffer;
		unsigned char* pixColor = _vertexColor;
		for (int i = 0; i < _bufHeight; i++)
		{
			for (int j = 0; j < _bufWidth; j++)
			{
				float v = *pixValue;
				unsigned char* c = (unsigned char*)pixColor;
				if (v < _minPixValue)
				{
					c[0] = 0;
					c[1] = 0;
					c[2] = 0;
				}
				else
				{
					_color->GetColor(c, c + 1, c + 2, (v - _minPixValue) / pixValueRange);
				}
				pixValue++;
				pixColor += 4;
			}
		}

		//以下初始化投影参数

		_pitchY = 0.0f;
		_rollX = -35.0f;
		_yawZ = 0.0f;

		_refLevel = (_minPixValue + _maxPixValue) / 2;

		float rangeX = abs(_halfRangeX)*2.0f;
		float rangeY = abs(_halfRangeY)*2.0f;

		//视场角45度，为了能够将数据全部放到视锥中
		_eyeZ = rangeX*tan(23.0) / 1.7 + rangeY;

		float r1 = rangeX / pixValueRange;
		float r2 = rangeY / pixValueRange;

		if (r1 < 2.0f || r2 < 2.0f)
			_scaleZ = 0.25f*(r1 > r2 ? r2 : r1);
	}

	void CalcVolume();

private:
	void DrawRefLevel();
	void DrawBoundary();

private:
	void OnPaint(wxPaintEvent& event);
	void OnSize(wxSizeEvent& event);
	void OnMouseEvent(wxMouseEvent& event);
	void OnWheelEvent(wxMouseEvent& event);
	void OnKeyDownEvent(wxKeyEvent& event);

	wxDECLARE_EVENT_TABLE();
};

wxRaster3dViewDialog::wxRaster3dViewDialog( wxWindow* parent, 
	wxWindowID id, 
	const wxString& title, 
	const wxPoint& pos, 
	const wxSize& size, 
	long style ) 
	: wxDialog( parent, id, title, pos, size, style )
{
	if (parent == nullptr)
		SetParent(((wxTGisApplication*)tGisApplication::INSTANCE())->GetMainFrame());

	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 1, 2, 0, 0 );
	fgSizer1->AddGrowableCol( 0 );
	fgSizer1->AddGrowableRow( 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	int wxGLAttribList[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 0 };

	_impl_ = new wxRaster3dViewDialogImpl(this, this, wxID_ANY, wxGLAttribList, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE | wxWANTS_CHARS);
	fgSizer1->Add(_impl_, 1, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );

	bSizer2->SetMinSize(wxSize(125, -1));
	wxArrayString _chBandChoices;
	_chBand = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, _chBandChoices, 0);
	_chBand->SetSelection(0);
	bSizer2->Add(_chBand, 0, wxALL | wxEXPAND, 5);

	_btnYawPlus = new wxButton(this, wxID_ANY, wxT("yaw+"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(_btnYawPlus, 0, wxALL | wxEXPAND, 5);

	_btnYawMinus = new wxButton(this, wxID_ANY, wxT("yaw-"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(_btnYawMinus, 0, wxALL | wxEXPAND, 5);

	_btnRollPlus = new wxButton(this, wxID_ANY, wxT("roll+"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(_btnRollPlus, 0, wxALL | wxEXPAND, 5);

	_btnRollMinus = new wxButton(this, wxID_ANY, wxT("roll-"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(_btnRollMinus, 0, wxALL | wxEXPAND, 5);

	_btnPitchPlus = new wxButton(this, wxID_ANY, wxT("pitch+"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(_btnPitchPlus, 0, wxALL | wxEXPAND, 5);

	_btnPitchMinus = new wxButton(this, wxID_ANY, wxT("pitch-"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(_btnPitchMinus, 0, wxALL | wxEXPAND, 5);

	_btnZoomIn = new wxButton(this, wxID_ANY, wxT("zoom in"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(_btnZoomIn, 0, wxALL | wxEXPAND, 5);

	_btnZoomOut = new wxButton(this, wxID_ANY, wxT("zoom out"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(_btnZoomOut, 0, wxALL | wxEXPAND, 5);

	_btnGradColor = new wxButton(this, wxID_ANY, wxT("Gradient Color"), wxDefaultPosition, wxDefaultSize, 0);
	bSizer2->Add(_btnGradColor, 0, wxALL | wxEXPAND, 5);

	_chkGradColor = new wxCheckBox(this, wxID_ANY, wxT("Gradient Color"), wxDefaultPosition, wxDefaultSize, 0);
	_chkGradColor->SetValue(true);
	bSizer2->Add(_chkGradColor, 0, wxALL, 5);

	_chkRefLevel = new wxCheckBox(this, wxID_ANY, wxT("Reference Level"), wxDefaultPosition, wxDefaultSize, 0);
	_chkRefLevel->SetValue(true);
	bSizer2->Add(_chkRefLevel, 0, wxALL, 5);

	_chkBoundary = new wxCheckBox(this, wxID_ANY, wxT("Boundary Ploygon"), wxDefaultPosition, wxDefaultSize, 0);
	_chkBoundary->SetValue(true);
	bSizer2->Add(_chkBoundary, 0, wxALL, 5);

	m_staticText1 = new wxStaticText(this, wxID_ANY, wxT("z stretch :"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText1->Wrap(-1);
	bSizer2->Add(m_staticText1, 0, wxALL, 5);

	_sldZStretch = new wxSlider(this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	bSizer2->Add(_sldZStretch, 0, wxALL | wxEXPAND, 5);

	_txtZStretch = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	bSizer2->Add(_txtZStretch, 0, wxALL | wxEXPAND, 5);

	m_staticText2 = new wxStaticText(this, wxID_ANY, wxT("reference level :"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText2->Wrap(-1);
	bSizer2->Add(m_staticText2, 0, wxALL, 5);

	_sldRefLevel = new wxSlider(this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	bSizer2->Add(_sldRefLevel, 0, wxALL | wxEXPAND, 5);

	_txtRefLevel = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
	bSizer2->Add(_txtRefLevel, 0, wxALL | wxEXPAND, 5);

	m_staticText3 = new wxStaticText(this, wxID_ANY, wxT("volume above :"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText3->Wrap(-1);
	bSizer2->Add(m_staticText3, 0, wxALL, 5);

	_txtVolumeAbove = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
	bSizer2->Add(_txtVolumeAbove, 0, wxALL | wxEXPAND, 5);

	m_staticText4 = new wxStaticText(this, wxID_ANY, wxT("volume below :"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText4->Wrap(-1);
	bSizer2->Add(m_staticText4, 0, wxALL, 5);

	_txtVolumeBelow = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
	bSizer2->Add(_txtVolumeBelow, 0, wxALL | wxEXPAND, 5);


	fgSizer1->Add( bSizer2, 0, wxEXPAND, 5 );


	this->SetSizer( fgSizer1 );
	this->Layout();

	this->Centre( wxBOTH );

	Bind(wxEVT_CHOICE, &wxRaster3dViewDialog::_chBand_Choice, this, _chBand->GetId());
	Bind(wxEVT_BUTTON, &wxRaster3dViewDialog::_btnYawPlus_Clicked, this, _btnYawPlus->GetId());
	Bind(wxEVT_BUTTON, &wxRaster3dViewDialog::_btnYawMinus_Clicked, this, _btnYawMinus->GetId());
	Bind(wxEVT_BUTTON, &wxRaster3dViewDialog::_btnRollPlus_Clicked, this, _btnRollPlus->GetId());
	Bind(wxEVT_BUTTON, &wxRaster3dViewDialog::_btnRollMinus_Clicked, this, _btnRollMinus->GetId());
	Bind(wxEVT_BUTTON, &wxRaster3dViewDialog::_btnPitchPlus_Clicked, this, _btnPitchPlus->GetId());
	Bind(wxEVT_BUTTON, &wxRaster3dViewDialog::_btnPitchMinus_Clicked, this, _btnPitchMinus->GetId());
	Bind(wxEVT_BUTTON, &wxRaster3dViewDialog::_btnZoomIn_Clicked, this, _btnZoomIn->GetId());
	Bind(wxEVT_BUTTON, &wxRaster3dViewDialog::_btnZoomOut_Clicked, this, _btnZoomOut->GetId());
	Bind(wxEVT_BUTTON, &wxRaster3dViewDialog::_btnGradColor_Clicked, this, _btnGradColor->GetId());
	Bind(wxEVT_CHECKBOX, &wxRaster3dViewDialog::_chkGradColor_Check, this, _chkGradColor->GetId());
	Bind(wxEVT_CHECKBOX, &wxRaster3dViewDialog::_chkRefLevel_Check, this, _chkRefLevel->GetId());
	Bind(wxEVT_CHECKBOX, &wxRaster3dViewDialog::_chkBoundary_Check, this, _chkBoundary->GetId());
	Bind(wxEVT_SLIDER, &wxRaster3dViewDialog::_sldZStretch_Scroll, this, _sldZStretch->GetId());
	Bind(wxEVT_SLIDER, &wxRaster3dViewDialog::_sldRefLevel_scroll, this, _sldRefLevel->GetId());
	Bind(wxEVT_TEXT_ENTER, &wxRaster3dViewDialog::_txtZStretch_TextEnter, this, _txtZStretch->GetId());
	Bind(wxEVT_TEXT_ENTER, &wxRaster3dViewDialog::_txtRefLevel_TextEnter, this, _txtRefLevel->GetId());
}

wxRaster3dViewDialog::~wxRaster3dViewDialog()
{
	Unbind(wxEVT_CHOICE, &wxRaster3dViewDialog::_chBand_Choice, this, _chBand->GetId());
	Unbind(wxEVT_BUTTON, &wxRaster3dViewDialog::_btnYawPlus_Clicked, this, _btnYawPlus->GetId());
	Unbind(wxEVT_BUTTON, &wxRaster3dViewDialog::_btnYawMinus_Clicked, this, _btnYawMinus->GetId());
	Unbind(wxEVT_BUTTON, &wxRaster3dViewDialog::_btnRollPlus_Clicked, this, _btnRollPlus->GetId());
	Unbind(wxEVT_BUTTON, &wxRaster3dViewDialog::_btnRollMinus_Clicked, this, _btnRollMinus->GetId());
	Unbind(wxEVT_BUTTON, &wxRaster3dViewDialog::_btnPitchPlus_Clicked, this, _btnPitchPlus->GetId());
	Unbind(wxEVT_BUTTON, &wxRaster3dViewDialog::_btnPitchMinus_Clicked, this, _btnPitchMinus->GetId());
	Unbind(wxEVT_BUTTON, &wxRaster3dViewDialog::_btnZoomIn_Clicked, this, _btnZoomIn->GetId());
	Unbind(wxEVT_BUTTON, &wxRaster3dViewDialog::_btnZoomOut_Clicked, this, _btnZoomOut->GetId());
	Unbind(wxEVT_BUTTON, &wxRaster3dViewDialog::_btnGradColor_Clicked, this, _btnGradColor->GetId());
	Unbind(wxEVT_CHECKBOX, &wxRaster3dViewDialog::_chkGradColor_Check, this, _chkGradColor->GetId());
	Unbind(wxEVT_CHECKBOX, &wxRaster3dViewDialog::_chkRefLevel_Check, this, _chkRefLevel->GetId());
	Unbind(wxEVT_CHECKBOX, &wxRaster3dViewDialog::_chkBoundary_Check, this, _chkBoundary->GetId());
	Unbind(wxEVT_SLIDER, &wxRaster3dViewDialog::_sldZStretch_Scroll, this, _sldZStretch->GetId());
	Unbind(wxEVT_SLIDER, &wxRaster3dViewDialog::_sldRefLevel_scroll, this, _sldRefLevel->GetId());
	Unbind(wxEVT_TEXT_ENTER, &wxRaster3dViewDialog::_txtZStretch_TextEnter, this, _txtZStretch->GetId());
	Unbind(wxEVT_TEXT_ENTER, &wxRaster3dViewDialog::_txtRefLevel_TextEnter, this, _txtRefLevel->GetId());
}

void wxRaster3dViewDialog::SetRaster(MyGDALRasterDataset * raster, IVertex2dList * polygon)
{
	_impl_->SetRaster(raster, polygon);

	_chBand->Clear();
	GDALDataset* dt = raster->GetGDALDataset();
	int layerCount = dt->GetRasterCount();
	for (int i = 1; i <= layerCount; i++)
	{
		wxString bandStr = wxString::Format(wxT("Band %d"), i);
		_chBand->Append(bandStr);
	}

	_chBand->SetSelection(0);
	wxCommandEvent e(wxEVT_CHOICE, _chBand->GetId());
	_chBand_Choice(e);
}

void wxRaster3dViewDialog::_chBand_Choice(wxCommandEvent & event)
{
	if ((_impl_->_bandIndex-1) == _chBand->GetSelection())
		return;
	_impl_->InitData(_chBand->GetSelection() + 1);
	_txtZStretch->SetValue(wxString::Format(wxT("%.3f"), _impl_->_scaleZ));
	_txtRefLevel->SetValue(wxString::Format(wxT("%.3f"), _impl_->_refLevel));
	_sldZStretch->SetValue(int(_impl_->_scaleZ*50));
	_impl_->CalcVolume();
	_txtVolumeAbove->SetValue(wxString::Format(wxT("%.5f"), _impl_->_volAbove));
	_txtVolumeBelow->SetValue(wxString::Format(wxT("%.5f"), _impl_->_volBelow));
	_impl_->Refresh(false);
}

void wxRaster3dViewDialog::_btnYawPlus_Clicked(wxCommandEvent & event)
{
	_impl_->_yawZ += 5.0f;
	if (_impl_->_yawZ >= 360.0f)
		_impl_->_yawZ = 0.0f;
	_impl_->Refresh(false);
}

void wxRaster3dViewDialog::_btnYawMinus_Clicked(wxCommandEvent & event)
{
	_impl_->_yawZ -= 5.0f;
	if (_impl_->_yawZ <= 0.0f)
		_impl_->_yawZ = 360.0f;
	_impl_->Refresh(false);
}

void wxRaster3dViewDialog::_btnRollPlus_Clicked(wxCommandEvent & event)
{
	_impl_->_rollX += 5.0f;
	_impl_->Refresh(false);
}

void wxRaster3dViewDialog::_btnRollMinus_Clicked(wxCommandEvent & event)
{
	_impl_->_rollX -= 5.0f;
	_impl_->Refresh(false);
}

void wxRaster3dViewDialog::_btnPitchPlus_Clicked(wxCommandEvent & event)
{
	_impl_->_pitchY += 5.0f;
	_impl_->Refresh(false);
}

void wxRaster3dViewDialog::_btnPitchMinus_Clicked(wxCommandEvent & event)
{
	_impl_->_pitchY -= 5.0f;
	_impl_->Refresh(false);
}

void wxRaster3dViewDialog::_btnZoomIn_Clicked(wxCommandEvent & event)
{
	_impl_->_eyeZ /= 1.2f;
	_impl_->Refresh(false);
}

void wxRaster3dViewDialog::_btnZoomOut_Clicked(wxCommandEvent & event)
{
	_impl_->_eyeZ *= 1.2f;
	_impl_->Refresh(false);
}

void wxRaster3dViewDialog::_btnGradColor_Clicked(wxCommandEvent & event)
{
	wxGradientColorPickerDialog dlg;
	dlg.SetGradientColor(_impl_->_color);
	if (dlg.ShowModal() == wxID_OK)
	{
		_impl_->_color->Release();
		_impl_->_color = dlg.GetGradientColor();
		_impl_->_color->Reference();

		double pixValueRange = _impl_->_maxPixValue - _impl_->_minPixValue;
		float* pixValue = _impl_->_pixBuffer;
		unsigned char* pixColor = _impl_->_vertexColor;
		for (int i = 0; i < _impl_->_bufHeight; i++)
		{
			for (int j = 0; j < _impl_->_bufWidth; j++)
			{
				float v = *pixValue;
				unsigned char* c = (unsigned char*)pixColor;
				if (v < _impl_->_minPixValue)
				{
					c[0] = 0;
					c[1] = 0;
					c[2] = 0;
				}
				else
				{
					_impl_->_color->GetColor(c, c + 1, c + 2, (v - _impl_->_minPixValue) / pixValueRange);
				}
				pixValue++;
				pixColor += 4;
			}
		}
		_impl_->Refresh(false);
	}
}

void wxRaster3dViewDialog::_chkGradColor_Check(wxCommandEvent & event)
{
	_impl_->_gradColorFill = _chkGradColor->GetValue();
	_impl_->Refresh(false);
}

void wxRaster3dViewDialog::_chkRefLevel_Check(wxCommandEvent & event)
{
	_impl_->_showRefLevel = _chkRefLevel->GetValue();
	_impl_->Refresh(false);
}

void wxRaster3dViewDialog::_chkBoundary_Check(wxCommandEvent & event)
{
	_impl_->_showBoundary = _chkBoundary->GetValue();
	_impl_->Refresh(false);
}

void wxRaster3dViewDialog::_sldZStretch_Scroll(wxCommandEvent & event)
{
	float pos = _sldZStretch->GetValue();
	_impl_->_scaleZ = 2.0f*pos / 100.0f;
	_txtZStretch->SetValue(wxString::Format(wxT("%.3f"), _impl_->_scaleZ));
	_impl_->Refresh(false);
}

void wxRaster3dViewDialog::_sldRefLevel_scroll(wxCommandEvent & event)
{
	float pos = _sldRefLevel->GetValue();
	_impl_->_refLevel = _impl_->_minPixValue + pos*(_impl_->_maxPixValue - _impl_->_minPixValue) / 100.0f;
	_txtRefLevel->SetValue(wxString::Format(wxT("%.3f"), _impl_->_refLevel));
	_impl_->Refresh(false);
	_impl_->CalcVolume();
	_txtVolumeAbove->SetValue(wxString::Format(wxT("%.5f"), _impl_->_volAbove));
	_txtVolumeBelow->SetValue(wxString::Format(wxT("%.5f"), _impl_->_volBelow));
}

void wxRaster3dViewDialog::_txtZStretch_TextEnter(wxCommandEvent& event)
{
	wxString strVal = _txtZStretch->GetValue();
	double scaleZ;
	if (strVal.ToDouble(&scaleZ))
	{
		_impl_->_scaleZ = scaleZ;
		_impl_->Refresh(false);
	}
}

void wxRaster3dViewDialog::_txtRefLevel_TextEnter(wxCommandEvent& event)
{
	wxString strVal = _txtZStretch->GetValue();
	double refLevel;
	if (strVal.ToDouble(&refLevel))
	{
		_impl_->_refLevel = refLevel;
		_impl_->Refresh(false);
		_impl_->CalcVolume();
		_txtVolumeAbove->SetValue(wxString::Format(wxT("%.5f"), _impl_->_volAbove));
		_txtVolumeBelow->SetValue(wxString::Format(wxT("%.5f"), _impl_->_volBelow));
	}
}


void wxRaster3dViewDialogImpl::CalcVolume()
{
	float aboveHeightSum = 0.0f;
	float belowHeightSum = 0.0f;

	float* pixValue = _pixBuffer;
	unsigned char* maskValue = _boundaryMask;
	for (int i = 0; i < _bufHeight; i++)
	{
		for (int j = 0; j < _bufWidth; j++)
		{
			float v = *pixValue;
			if (*maskValue > 0 && v >= _minPixValue)
			{
				if (v >= _refLevel)
					aboveHeightSum += (v - _refLevel);
				else
					belowHeightSum += (_refLevel - v);
			}

			pixValue++;
			maskValue++;
		}
	}

	_volAbove = _bufPixArea*aboveHeightSum;
	_volBelow = _bufPixArea*belowHeightSum;
}

void wxRaster3dViewDialogImpl::DrawRefLevel()
{
	float x1 = -_halfRangeX;
	float y1 = -_halfRangeY;
	float x2 = _halfRangeX;
	float y2 = _halfRangeY;
	float h = _refLevel - _minPixValue;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);

	glColor4f(0.0f, 0.85f, 0.13f, 0.65f);

	glVertex3f(x1, y1, h);
	glVertex3f(x2, y1, h);
	glVertex3f(x2, y2, h);
	glVertex3f(x1, y2, h);

	glEnd();
}

void wxRaster3dViewDialogImpl::DrawBoundary()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_QUAD_STRIP);

	glColor4f(0.95f, 0.95f, 0.0f, 0.65f);
	size_t vertexCount = _polygon->GetVertexCount();
	double firstX;
	double firstY;
	double h = _maxPixValue - _minPixValue;
	for (size_t i = 0; i < vertexCount; i++)
	{
		double x, y;
		_polygon->GetVertex(i, &x, &y);
		x = x - _startX - _halfRangeX;
		y = y - _startY - _halfRangeY;
		glVertex3d(x, y, 0.0);
		glVertex3d(x, y, h);
		if (i == 0)
		{
			firstX = x;
			firstY = y;
		}
	}
	glVertex3d(firstX, firstY, 0.0);
	glVertex3d(firstX, firstY, h);
	glEnd();
}

void wxRaster3dViewDialogImpl::OnPaint(wxPaintEvent & event)
{
	wxPaintDC dc(this);
	_glContext.SetCurrent(*this);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	wxSize sz = GetClientSize();
	if (sz.x == 0 || sz.y == 0)
	{
		event.Skip();
		return;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective_(45.0f, float(sz.x) / float(sz.y), 1.0f, _eyeZ * 10);
	glViewport(0, 0, sz.x, sz.y);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//glDrawBuffer(GL_BACK);

	gluLookAt_(0.0f, 0.0f, _eyeZ, _seeX, _seeY, 0.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(_rollX, 1.0f, 0.0f, 0.0f);//	
	glRotatef(_yawZ, 0.0f, 0.0f, 1.0f);//
	glRotatef(_pitchY, 0.0f, 1.0f, 0.0f);//

	glScalef(1.0f, 1.0f, _scaleZ);

	if(_gradColorFill)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glColorMaterial(GL_FRONT_AND_BACK, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	float* pixLine1 = _pixBuffer;
	float* pixLine2 = _pixBuffer + _pixWidth;
	unsigned char* colorLine1 = _vertexColor;
	unsigned char* colorLine2 = _vertexColor + _pixWidth * 4;

	float x = -_halfRangeX;
	float y = -_halfRangeY;
	float* triNormalValue = _triNormal;
	float vertex1[3];
	float vertex2[3];
	float vertex3[3];
	float vertex4[3];

	for (int i = 0; i < _bufHeight - 1; i++)
	{
		for (int j = 0; j < _bufWidth - 1; j++)
		{
			vertex1[0] = x;
			vertex2[0] = x + _bufStepX;
			vertex3[0] = x + _bufStepX;
			vertex4[0] = x;

			vertex1[1] = y;
			vertex2[1] = y;
			vertex3[1] = y + _bufStepY;
			vertex4[1] = y + _bufStepY;

			vertex1[2] = *pixLine1 - _minPixValue;
			vertex2[2] = *(pixLine1 + 1) - _minPixValue;
			vertex3[2] = *(pixLine2 + 1) - _minPixValue;
			vertex4[2] = *pixLine2 - _minPixValue;

			unsigned char* color1 = colorLine1;
			unsigned char* color2 = colorLine1 + 4;
			unsigned char* color3 = colorLine2 + 4;
			unsigned char* color4 = colorLine2;

			if (_gradColorFill)
			{
				glBegin(GL_TRIANGLES);

				glNormal3fv(triNormalValue);
				glColor3ub(color1[0], color1[1], color1[2]);
				glVertex3f(vertex1[0], vertex1[1], vertex1[2]);
				glColor3ub(color2[0], color2[1], color2[2]);
				glVertex3f(vertex2[0], vertex2[1], vertex2[2]);
				glColor3ub(color4[0], color4[1], color4[2]);
				glVertex3f(vertex4[0], vertex4[1], vertex4[2]);

				triNormalValue += 3;

				glNormal3fv(triNormalValue);
				glColor3ub(color2[0], color2[1], color2[2]);
				glVertex3f(vertex2[0], vertex2[1], vertex2[2]);
				glColor3ub(color3[0], color3[1], color3[2]);
				glVertex3f(vertex3[0], vertex3[1], vertex3[2]);
				glColor3ub(color4[0], color4[1], color4[2]);
				glVertex3f(vertex4[0], vertex4[1], vertex4[2]);

				triNormalValue += 3;

				glEnd();
			}
			else
			{
				glBegin(GL_TRIANGLES);
				glColor3f(0.2f, 1.0f, 0.2f);

				glNormal3fv(triNormalValue);
				glVertex3f(vertex1[0], vertex1[1], vertex1[2]);
				glVertex3f(vertex2[0], vertex2[1], vertex2[2]);
				glVertex3f(vertex4[0], vertex4[1], vertex4[2]);

				triNormalValue += 3;

				glNormal3fv(triNormalValue);
				glVertex3f(vertex2[0], vertex2[1], vertex2[2]);
				glVertex3f(vertex3[0], vertex3[1], vertex3[2]);
				glVertex3f(vertex4[0], vertex4[1], vertex4[2]);

				triNormalValue += 3;

				glEnd();
			}

			pixLine1++;
			pixLine2++;
			colorLine1 += 4;
			colorLine2 += 4;

			x += _bufStepX;
		}

		pixLine1++;
		pixLine2++;
		colorLine1 += 4;
		colorLine2 += 4;

		y += _bufStepY;
		x = -_halfRangeX;
	}

	if(_showRefLevel)
		DrawRefLevel();

	if (_showBoundary)
		DrawBoundary();

	SwapBuffers();
	event.Skip();
}

void wxRaster3dViewDialogImpl::OnSize(wxSizeEvent & event)
{
	Refresh(false);
	event.Skip();
}

void wxRaster3dViewDialogImpl::OnMouseEvent(wxMouseEvent & event)
{
	//SetFocus();
	event.Skip();
}

void wxRaster3dViewDialogImpl::OnWheelEvent(wxMouseEvent & e)
{
	int degree = e.GetWheelDelta();

	if (degree == 0)
		return;

	int dir = e.GetWheelRotation();
	if (dir < 0)
		degree = -degree;

	int step = (int)(abs(degree / 120.0));
	step = step == 0 ? 1 : step;

	if (e.ShiftDown())
		step = step * 2;

	double scale = 1.0f;

	for (int i = 0; i < step; i++)
	{
		if (degree > 0)
		{
			scale /= 1.05; //360/375
		}
		else
		{
			scale *= 1.05; //375/360
		}
	}

	_eyeZ *= scale;
	Refresh(false);

	e.Skip();
}

void wxRaster3dViewDialogImpl::OnKeyDownEvent(wxKeyEvent & e)
{
	int key = e.GetKeyCode();

	switch (key)
	{
		//roll
	case WXK_UP:
		_rollX -= 5.0f;
		break;
	case WXK_DOWN:
		_rollX += 5.0f;
		break;
		//yaw
	case WXK_LEFT:
		_yawZ -= 5.0f;
		break;
	case WXK_RIGHT:
		_yawZ += 5.0f;
		break;
		//移动视点
	case 87: //W
		_seeY += _bufStepY;
		break;
	case 83: //S
		_seeY -= _bufStepY;
		break;
	case 65://A
		_seeX += _bufStepX;
		break;
	case 68://D
		_seeX -= _bufStepX;
		break;
	default:
		break;
	}
	Refresh(false);
	e.Skip();
}

wxBEGIN_EVENT_TABLE(wxRaster3dViewDialogImpl, wxGLCanvas)
    EVT_PAINT(wxRaster3dViewDialogImpl::OnPaint)
	EVT_SIZE(wxRaster3dViewDialogImpl::OnSize)
	EVT_MOUSE_EVENTS(wxRaster3dViewDialogImpl::OnMouseEvent)
	EVT_MOUSEWHEEL(wxRaster3dViewDialogImpl::OnWheelEvent)
	EVT_KEY_DOWN(wxRaster3dViewDialogImpl::OnKeyDownEvent)
wxEND_EVENT_TABLE()

END_NAME_SPACE(tGis, Gui)
