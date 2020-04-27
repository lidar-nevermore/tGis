#include "wxGLGeoSurface.h"
#include "wxGLMapWidget.h"

#include <GL/gl.h>

BEGIN_NAME_SPACE(tGis, Gui)

wxGLGeoSurface::wxGLGeoSurface(wxGLMapWidget* mapWidget, bool extraBuffer)
	:_glContext(mapWidget)
{
	_mapWidget = mapWidget;
	_glContext.SetCurrent(*_mapWidget);
	_mapBuffer = nullptr;
	_extraBuffer = extraBuffer;
	_bufWidth = 0;
	_bufHeight = 0;
}


wxGLGeoSurface::~wxGLGeoSurface()
{
	if (_mapBuffer != nullptr)
		free(_mapBuffer);
}

void wxGLGeoSurface::Present(int wX, int wY)
{
	Present(wX, wY, _mapWidth, _mapHeight);
}

void wxGLGeoSurface::Present(int wX, int wY, int wW, int wH)
{
	if (_mapBuffer == nullptr)
		return;

	wxSize sz = _mapWidget->GetClientSize();
	//目标范围的NDC坐标
	float left = (2.0f*wX) / sz.x - 1.0;
	float right = (2.0f*(wX + wW)) / sz.x - 1.0;
	float bottom = 1.0 - (2.0f*wY) / sz.y;
	float top = 1.0 - (2.0f*(wY + wH)) / sz.y;

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _mapWidth, _mapHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, _mapBuffer);  //载入纹理：																										

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//正射投影
	glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);    //启用2D纹理映射
	glColor4f(_mapWidget->_br, _mapWidget->_bg, _mapWidget->_bb, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(left, bottom, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(right, bottom, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(right, top, 0.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(left, top, 0.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void wxGLGeoSurface::BeginPaint(bool isCache)
{
	_glContext.SetCurrent(*_mapWidget);
	GeoSurface::BeginPaint(isCache);
	//TODO: 设置OpenGL参数
	int surfWidth;
	int surfHeight;
	_viewPort.GetSurfaceSize(&surfWidth, &surfHeight);
	glViewport(0, 0, surfWidth, surfHeight);
	glClearColor(_mapWidget->_br, _mapWidget->_bg, _mapWidget->_bb, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void wxGLGeoSurface::EndPaint(bool isCache)
{
	int surfWidth;
	int surfHeight;
	_viewPort.GetSurfaceSize(&surfWidth, &surfHeight);

	if (_extraBuffer && isCache == false && surfHeight > 0 && surfWidth > 0)
	{
		glFinish();

		if (surfWidth > _bufWidth || surfHeight > _bufHeight)
		{
			if (_mapBuffer != nullptr)
				free(_mapBuffer);
			_mapBuffer = malloc(surfWidth*surfHeight*4);
			_bufWidth = surfWidth;
			_bufHeight = surfHeight;
		}
		_mapWidth = surfWidth;
		_mapHeight = surfHeight;
		//创建图片缓冲
		glReadBuffer(GL_BACK);
		glReadPixels(0, 0, _mapWidth, _mapHeight, GL_RGBA, GL_UNSIGNED_BYTE, _mapBuffer);
	}

	IOverlayLayer* overlayLayer = _mapWidget->GetOverlayLayer();
	overlayLayer->Paint((IGeoSurface*)this);

	//if (widget->_gridVisible)
	//{
	//	int wW, wH;
	//	widget->GetClientSize(&wW, &wH);
	//	for (int x = 25; x < wW; x += 25)
	//	{
	//		painter.drawLine(x, 0, x, wH);
	//	}
	//	for (int y = 25; y < wH; y += 25)
	//	{
	//		painter.drawLine(0, y, wW, y);
	//	}
	//}
	glFinish();
}

void wxGLGeoSurface::DrawImage(const unsigned char* buf, int width, int height, int surfX, int surfY)
{
	DrawImage(buf, width, height, surfX, surfY, width, height);
}

void wxGLGeoSurface::DrawImage(const unsigned char* buf, int width, int height, int surfX, int surfY, int surfW, int surfH)
{
	int surfWidth;
	int surfHeight;
	_viewPort.GetSurfaceSize(&surfWidth, &surfHeight);
	
	//目标范围的NDC坐标
	float left = (2.0f*surfX) / surfWidth - 1.0;
	float right = (2.0f*(surfX + surfW)) / surfWidth - 1.0;
	float bottom = 1.0 - (2.0f*surfY) / surfHeight;
	float top = 1.0 - (2.0f*(surfY + surfH)) / surfHeight;

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf);  //载入纹理：																										

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//正射投影
	glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);    //启用2D纹理映射
	glColor4f(_mapWidget->_br, _mapWidget->_bg, _mapWidget->_bb, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f(left, bottom, 0.0f);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f(right, bottom, 0.0f);
	glTexCoord2f(1.0f, 1.0f);
	glVertex3f(right, top, 0.0f);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(left, top, 0.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

END_NAME_SPACE(tGis, Gui)
