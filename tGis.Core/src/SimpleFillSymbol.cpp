#include "SimpleFillSymbol.h"
#include "SimpleSymbolLibrary.h"
#include <memory>

#include "ISurface.h"

#include "glad.h"
#include "glutess.h"

extern "C"
{
#include "bi_list.h"
}

namespace Strippe
{
#include "DenseDot1.inl"
#include "DenseDot2.inl"
#include "DenseDot3.inl"
#include "DenseDot4.inl"
#include "DenseDot5.inl"
#include "DenseDot6.inl"
#include "DenseDot7.inl"
#include "Horizontal.inl"
#include "Vertical.inl"
#include "ForwardDiagonal.inl"
#include "BackwardDiagonal.inl"
#include "Cross.inl"
#include "DiagonalCross.inl"
}

BEGIN_NAME_SPACE(tGis, Core)

class gluTessHelper
{
private:
	static gluTessHelper* _instance;

public:
	static inline gluTessHelper* GetInstance()
	{
		if (_instance == nullptr)
		{
			_instance = new gluTessHelper();
			static std::unique_ptr<gluTessHelper> shit(_instance);
		}

		return _instance;
	}

	static GLUtesselator* GetTesselator()
	{
		return GetInstance()->_tesselator;
	}

	static GLdouble* TessVertex()
	{
		return (GLdouble*)bi_list_insert_head(&(GetInstance()->_newVertexList), nullptr);
	}

	static void TessEnd()
	{
		bi_list_clear(&(GetInstance()->_newVertexList));
	}

	static const GLubyte* GetStrippe(int id)
	{
		return GetInstance()->_strippes[id];
	}


	~gluTessHelper()
	{
		gluDeleteTess(_tesselator);
		bi_list_release(&_newVertexList);
	}

private:
	static void GLAPIENTRY tGisCombine(
		const GLdouble newVertex[3],
		const GLdouble *neighborVertex[4],
		const GLfloat neighborWeight[4],
		GLdouble **outData)
	{
		*outData = (GLdouble*)bi_list_insert_head(&(_instance->_newVertexList), newVertex);
	}

private:
	gluTessHelper()
	{
		_tesselator = gluNewTess();
		gluTessCallback(_tesselator, GLU_TESS_BEGIN, (_GLUfuncptr)glBegin);
		gluTessCallback(_tesselator, GLU_TESS_END, (_GLUfuncptr)glEnd);
		gluTessCallback(_tesselator, GLU_TESS_VERTEX, (_GLUfuncptr)glVertex3dv);
		gluTessCallback(_tesselator, GLU_TESS_COMBINE, (_GLUfuncptr)tGisCombine);
		gluTessProperty(_tesselator, GLU_TESS_TOLERANCE, GL_TRUE);
		gluTessProperty(_tesselator, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);

		bi_list_init(&_newVertexList,
			3 * sizeof(GLdouble), //自相交形成的顶点
			malloc,
			free,
			nullptr,
			nullptr);

		_strippes[0] = nullptr;
		_strippes[1] = nullptr;
		_strippes[2] = Strippe::DenseDot1;
		_strippes[3] = Strippe::DenseDot2;
		_strippes[4] = Strippe::DenseDot3;
		_strippes[5] = Strippe::DenseDot4;
		_strippes[6] = Strippe::DenseDot5;
		_strippes[7] = Strippe::DenseDot6;
		_strippes[8] = Strippe::DenseDot7;
		_strippes[9] = Strippe::Horizontal;
		_strippes[10] = Strippe::Vertical;
		_strippes[11] = Strippe::ForwardDiagonal;
		_strippes[12] = Strippe::BackwardDiagonal;
		_strippes[13] = Strippe::Cross;
		_strippes[14] = Strippe::DiagonalCross;
	}

	GLUtesselator* _tesselator;
	BI_LIST _newVertexList;
	const GLubyte* _strippes[SimpleFillSymbol::MaxId + 1];
};

gluTessHelper* gluTessHelper::_instance = nullptr;

SimpleFillSymbol::SimpleFillSymbol()
	:SimpleFillSymbol(SimpleFillSymbol::DiagonalCross, nullptr)
{
	_lib = nullptr;
}

SimpleFillSymbol::SimpleFillSymbol(unsigned char r, unsigned char g, unsigned char b, unsigned char a, int t)
	: IFillSymbol(nullptr)
{
	_type = t;
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}


SimpleFillSymbol::~SimpleFillSymbol()
{
}

SimpleFillSymbol::SimpleFillSymbol(int t, const ISymbolLibrary * symLib)
	:IFillSymbol(symLib)
{
	_type = t;
	_r = 30;
	_g = 236;
	_b = 63;
	_a = 255;
}

int SimpleFillSymbol::GetId()
{
	return _type;
}


void SimpleFillSymbol::Paint(ISurface * surf, int count, int * x, int * y)
{
	if (_type == NoFill)
		return;

	size_t ndcSize = count * 3 * sizeof(GLdouble);
	GLdouble* ndc = (GLdouble*)malloc(ndcSize);

	//if (ndc == nullptr)
	//	return;

	GLfloat red = _r / 255.0f;
	GLfloat green = _g / 255.0f;
	GLfloat blue = _b / 255.0f;
	GLfloat alpha = _a / 255.0f;

	glColor4f(red, green, blue, alpha);
	if (_type > 1)
	{
		glEnable(GL_POLYGON_STIPPLE);
		glPolygonStipple(gluTessHelper::GetStrippe(_type));
	}

	GLUtesselator* tess = gluTessHelper::GetTesselator();
	GLdouble* ndc1 = ndc;
	gluTessBeginPolygon(tess, nullptr);
	gluTessBeginContour(tess);
	for (int i = 0; i < count; i++)
	{
		ndc1[2] = 0.0;
		surf->Surface2glndc(x[i], y[i], ndc1, ndc1 +1);
		gluTessVertex(tess, ndc1, ndc1);
		ndc1 += 3;
	}
	gluTessEndContour(tess);
	gluTessEndPolygon(tess);
	free(ndc);
	gluTessHelper::TessEnd();
	if (_type > 1)
		glDisable(GL_POLYGON_STIPPLE);
}

void SimpleFillSymbol::Paint(ISurface* surf, int contourCount, int* ptCount, int** x, int** y)
{
	if (_type == NoFill)
		return;

	size_t ndcSize = 0;
	for (int c = 0; c < contourCount; c++)
		ndcSize += (ptCount[c] * 3 * sizeof(GLdouble));
	GLdouble* ndc = (GLdouble*)malloc(ndcSize);


	GLfloat red = _r / 255.0f;
	GLfloat green = _g / 255.0f;
	GLfloat blue = _b / 255.0f;
	GLfloat alpha = _a / 255.0f;

	glColor4f(red, green, blue, alpha);
	if (_type > 1)
	{
		glEnable(GL_POLYGON_STIPPLE);
		glPolygonStipple(gluTessHelper::GetStrippe(_type));
	}
	GLUtesselator* tess = gluTessHelper::GetTesselator();
	GLdouble* ndc1 = ndc;
	gluTessBeginPolygon(tess, 0);

	for (int c = 0; c < contourCount; c++)
	{
		int* xc = x[c];
		int* yc = y[c];
		gluTessBeginContour(tess);
		for (int i = 0; i < ptCount[c]; i++)
		{
			ndc1[2] = 0.0;
			surf->Surface2glndc(xc[i], yc[i], ndc1, ndc1 + 1);
			gluTessVertex(tess, ndc1, ndc1);
			ndc1 += 3;
		}
		gluTessEndContour(tess);
	}

	gluTessEndPolygon(tess);

	free(ndc);
	gluTessHelper::TessEnd();
	if (_type > 1)
		glDisable(GL_POLYGON_STIPPLE);
}

void SimpleFillSymbol::BeginPaint(ISurface * surf)
{
	if (_type == NoFill)
		return;

	int surfWidth;
	int surfHeight;
	surf->GetSize(&surfWidth, &surfHeight);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f, (GLfloat)surfWidth, (GLfloat)surfHeight, 0.0f, -1.0f, 1.0f);

	GLfloat red = _r / 255.0f;
	GLfloat green = _g / 255.0f;
	GLfloat blue = _b / 255.0f;
	GLfloat alpha = _a / 255.0f;

	glColor4f(red, green, blue, alpha);
	if (_type > 1)
	{
		glEnable(GL_POLYGON_STIPPLE);
		glPolygonStipple(gluTessHelper::GetStrippe(_type));
	}

	GLUtesselator* tess = gluTessHelper::GetTesselator();
	gluTessBeginPolygon(tess, nullptr);
}

void SimpleFillSymbol::BeginContour(ISurface * surf)
{
	if (_type == NoFill)
		return;

	GLUtesselator* tess = gluTessHelper::GetTesselator();
	gluTessBeginContour(tess);
}

void SimpleFillSymbol::AppendVertex(int count, int * x, int * y)
{
	if (_type == NoFill)
		return;

	GLUtesselator* tess = gluTessHelper::GetTesselator();

	for (int i = 0; i < count; i++)
	{
		GLdouble* vertex = gluTessHelper::TessVertex();
		vertex[2] = (GLdouble)0.0;
		vertex[0] = (GLdouble)x[i];
		vertex[1] = (GLdouble)y[i];
		gluTessVertex(tess, vertex, vertex);
	}
}

void SimpleFillSymbol::AppendVertex(int x, int y)
{
	if (_type == NoFill)
		return;

	GLUtesselator* tess = gluTessHelper::GetTesselator();
	GLdouble* vertex = gluTessHelper::TessVertex();
	vertex[0] = (GLdouble)x;
	vertex[1] = (GLdouble)y;
	vertex[2] = (GLdouble)0.0;
	gluTessVertex(tess, vertex, vertex);
}

void SimpleFillSymbol::EndContour(ISurface * surf)
{
	if (_type == NoFill)
		return;

	GLUtesselator* tess = gluTessHelper::GetTesselator();
	gluTessEndContour(tess);
}

void SimpleFillSymbol::EndPaint(ISurface * surf)
{
	if (_type == NoFill)
		return;

	GLUtesselator* tess = gluTessHelper::GetTesselator();
	gluTessEndPolygon(tess);

	gluTessHelper::TessEnd();
	if (_type > 1)
		glDisable(GL_POLYGON_STIPPLE);
	glPopMatrix();
}

void SimpleFillSymbol::GetColor(unsigned char * r, unsigned char * g, unsigned char * b, unsigned char * a)
{
	*r = _r;
	*g = _g;
	*b = _b;
	*a = _a;
}

void SimpleFillSymbol::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}


END_NAME_SPACE(tGis, Core)

