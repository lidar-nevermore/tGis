#include "wxGLGeoSurface.h"
#include "wxGLMapWidget.h"

#include <GL/gl.h>

BEGIN_NAME_SPACE(tGis, Gui)

wxGLGeoSurface::wxGLGeoSurface(wxGLMapWidget* mapWidget)
	:_glContext(mapWidget)
{
	_mapWidget = mapWidget;
	_glContext.SetCurrent(*_mapWidget);
	//TODO: 设置OpenGL参数
}


wxGLGeoSurface::~wxGLGeoSurface()
{
}

void wxGLGeoSurface::Present(IWidget * w, int wX, int wY)
{
}

void wxGLGeoSurface::Present(IWidget * w, int wX, int wY, int wW, int wH)
{
}

void wxGLGeoSurface::BeginPaint(IWidget * w, bool isCache)
{
	_glContext.SetCurrent(*_mapWidget);
	GeoSurface::BeginPaint(w, isCache);
	//TODO: 设置OpenGL参数
	int surfWidth;
	int surfHeight;
	_viewPort.GetSurfaceSize(&surfWidth, &surfHeight);
	glViewport(0, 0, surfWidth, surfHeight);
	float red = _mapWidget->_backgroundR / 255.0;
	float green = _mapWidget->_backgroundG / 255.0;
	float blue = _mapWidget->_backgroundB / 255.0;
	glClearColor(red, green, blue, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void wxGLGeoSurface::EndPaint(IWidget * w, bool isCache)
{
	glFlush();
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
