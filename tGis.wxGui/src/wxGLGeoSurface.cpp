#include "wxGLGeoSurface.h"
#include "wxGLMapWidget.h"

#include <GL/gl.h>

BEGIN_NAME_SPACE(tGis, Gui)

wxGLGeoSurface::wxGLGeoSurface(wxGLMapWidget* mapWidget)
	:_glContext(mapWidget)
{
	_mapWidget = mapWidget;
	_glContext.SetCurrent(*_mapWidget);
	//TODO: ����OpenGL����
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
	//TODO: ����OpenGL����
	int surfWidth;
	int surfHeight;
	_viewPort.GetSurfaceSize(&surfWidth, &surfHeight);
	glViewport(0, 0, surfWidth, surfHeight);
	glClearColor(_mapWidget->_br, _mapWidget->_bg, _mapWidget->_bb, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
}

void wxGLGeoSurface::EndPaint(IWidget * w, bool isCache)
{
	//if (isCache == false)
	//{
	//	glFinish();
	//	//����ͼƬ����
	//}

	wxGLMapWidget* widget = (wxGLMapWidget*)w;
	IOverlayLayer* overlayLayer = widget->GetOverlayLayer();
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
	
	//Ŀ�귶Χ��NDC����
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
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buf);  //��������																										

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//����ͶӰ
	glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

	glEnable(GL_TEXTURE_2D);    //����2D����ӳ��
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
