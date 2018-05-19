#include "QtGeoSurface.h"


QtGeoSurface::QtGeoSurface()
{
	_painter = nullptr;
	_osSurf4Paint = nullptr;
	_osSurf4Present = nullptr;
}


QtGeoSurface::~QtGeoSurface()
{
}

void QtGeoSurface::SetBackgroundColor(unsigned char R, unsigned char G, unsigned char B)
{
	_surfBackgroundR = R;
	_surfBackgroundG = G;
	_surfBackgroundB = B;
}

void QtGeoSurface::EnsurePaintSurfaceValid()
{
	int psw = 0;
	int psh = 0;
	if (_osSurf4Paint != nullptr)
	{
		QSize sz = _osSurf4Paint->size();
		psw = sz.width();
		psh = sz.height();
	}
	if (_surfWidth > psw && _surfHeight > psh)
	{
		if (_osSurf4Paint != nullptr)
			delete _osSurf4Paint;
		_osSurf4Paint = new QPixmap(_surfWidth, _surfHeight);
		_osSurf4Paint->fill(QColor(_surfBackgroundR, _surfBackgroundG, _surfBackgroundB));
	}
}

void QtGeoSurface::AttachQPainter(QPainter * painter)
{
	_painter = painter;
}

void QtGeoSurface::DetachQPainter()
{
	_painter = nullptr;
}

void QtGeoSurface::PresentSurface()
{
	if(_osSurf4Present != nullptr)
		_painter->drawPixmap(_osSurfPresentPosX, _osSurfPresentPosY, _osSurfPresentWidth, _osSurfPresentHeight, *_osSurf4Present, 0, 0, _osSurf4PresentWidth, _osSurf4PresentHeight);
}

void QtGeoSurface::SwithSurface()
{
	GeoSurface::SwithSurface();
	if (_osSurf4Present != nullptr)
	{
		delete _osSurf4Present;
	}
	_osSurf4Present = _osSurf4Paint;
	_osSurf4Paint = nullptr;
}

void QtGeoSurface::DrawPolyline(int count, int * surfX, int * surfY, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt)
{
	EnsurePaintSurfaceValid();
	if (_osSurf4Paint == nullptr)
		return;
}

void QtGeoSurface::DrawPolygon(int count, int * surfX, int * surfY, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt)
{
	EnsurePaintSurfaceValid();
	if (_osSurf4Paint == nullptr)
		return;
}

void QtGeoSurface::FillPolygon(int count, int * surfX, int * surfY, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int ft)
{
	EnsurePaintSurfaceValid();
	if (_osSurf4Paint == nullptr)
		return;
}

void QtGeoSurface::DrawEllipse(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt)
{
	EnsurePaintSurfaceValid();
	if (_osSurf4Paint == nullptr)
		return;
}

void QtGeoSurface::FillEllipse(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int ft)
{
	EnsurePaintSurfaceValid();
	if (_osSurf4Paint == nullptr)
		return;
}

void QtGeoSurface::DrawRect(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt)
{
	EnsurePaintSurfaceValid();
	if (_osSurf4Paint == nullptr)
		return;
}

void QtGeoSurface::FillRect(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int ft)
{
	EnsurePaintSurfaceValid();
	if (_osSurf4Paint == nullptr)
		return;
}

void QtGeoSurface::DrawImage(const unsigned char * buf, int surfX, int surfY, int width, int height)
{
	EnsurePaintSurfaceValid();
	if(_osSurf4Paint != nullptr)
	{
		QPainter painter(_osSurf4Paint);
		QImage img(buf, width, height, QImage::Format_ARGB32);
		painter.drawImage(QRect(surfX, surfY,width,height), img, QRect(0, 0, width, height));
	}
}
