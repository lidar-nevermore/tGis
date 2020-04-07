#include "wxGLGeoSurface.h"


BEGIN_NAME_SPACE(tGis, wxGui)

wxGLGeoSurface::wxGLGeoSurface()
{
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
}

void wxGLGeoSurface::EndPaint(IWidget * w, bool isCache)
{
}

void wxGLGeoSurface::DrawPolyline(int count, int * surfX, int * surfY, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt)
{
}

void wxGLGeoSurface::DrawPolygon(int count, int * surfX, int * surfY, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt)
{
}

void wxGLGeoSurface::FillPolygon(int count, int * surfX, int * surfY, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int ft)
{
}

void wxGLGeoSurface::DrawEllipse(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt)
{
}

void wxGLGeoSurface::FillEllipse(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int ft)
{
}

void wxGLGeoSurface::DrawRect(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt)
{
}

void wxGLGeoSurface::FillRect(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int ft)
{
}

void wxGLGeoSurface::DrawImage(const unsigned char * buf, int surfX, int surfY, int width, int height)
{
}

END_NAME_SPACE(tGis, wxGui)
