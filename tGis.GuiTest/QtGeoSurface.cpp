#include "QtGeoSurface.h"
#include "SimpleLineSymbol.h"
#include "SimpleFillSymbol.h"


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

inline void QtGeoSurface::EnsurePaintSurfaceValid()
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

inline Qt::PenStyle QtGeoSurface::TranslateLineStyle(int lt)
{
	switch (lt)
	{
	case SimpleLineSymbol::Solid:
		return Qt::SolidLine;
	case SimpleLineSymbol::Dash:
		return Qt::DashLine;
	case SimpleLineSymbol::Dot:
		return Qt::DotLine;
	case SimpleLineSymbol::DashDot:
		return Qt::DashDotLine;
	case SimpleLineSymbol::DashDotDot:
		return Qt::DashDotDotLine;
	default:
		return Qt::SolidLine;
	}
}

inline Qt::BrushStyle QtGeoSurface::TranslateFillStyle(int ft)
{
	switch (ft)
	{
	case SimpleFillSymbol::NoFill:
		return Qt::NoBrush;
	case SimpleFillSymbol::Solid:
		return Qt::SolidPattern;
	case SimpleFillSymbol::Dense1:
		return Qt::Dense1Pattern;
	case SimpleFillSymbol::Dense2:
		return Qt::Dense2Pattern;
	case SimpleFillSymbol::Dense3:
		return Qt::Dense3Pattern;
	case SimpleFillSymbol::Dense4:
		return Qt::Dense4Pattern;
	case SimpleFillSymbol::Dense5:
		return Qt::Dense5Pattern;
	case SimpleFillSymbol::Dense6:
		return Qt::Dense6Pattern;
	case SimpleFillSymbol::Dense7:
		return Qt::Dense7Pattern;
	case SimpleFillSymbol::Horizontal:
		return Qt::HorPattern;
	case SimpleFillSymbol::Vertical:
		return Qt::VerPattern;
	case SimpleFillSymbol::ForwardDiagonal:
		return Qt::FDiagPattern;
	case SimpleFillSymbol::BackwardDiagonal:
		return Qt::BDiagPattern;
	case SimpleFillSymbol::Cross:
		return Qt::CrossPattern;
	case SimpleFillSymbol::DiagonalCross:
		return Qt::DiagCrossPattern;
	default:
		return Qt::NoBrush;
	}
}

inline QPoint * QtGeoSurface::CreateQPoints(int count, int * surfX, int * surfY)
{
	QPoint* qpt = new QPoint[count];
	for (int i = 0; i < count; i++)
	{
		qpt[i].setX(surfX[i]);
		qpt[i].setY(surfY[i]);
	}

	return qpt;
}

inline void QtGeoSurface::DeleteQPoints(QPoint * pts)
{
	delete[] pts;
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

	QPoint* qpt = CreateQPoints(count, surfX, surfX);

	QPainter painter(_osSurf4Paint);
	QPen pen(QColor(r, g, b, a), lw, TranslateLineStyle(lt));
	painter.setPen(pen);
	painter.setBrush(Qt::NoBrush);
	painter.drawPolyline(qpt, count);

	DeleteQPoints(qpt);
}

void QtGeoSurface::DrawPolygon(int count, int * surfX, int * surfY, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt)
{
	EnsurePaintSurfaceValid();
	if (_osSurf4Paint == nullptr)
		return;

	QPoint* qpt = CreateQPoints(count, surfX, surfX);

	QPainter painter(_osSurf4Paint);
	QPen pen(QColor(r, g, b, a), lw, TranslateLineStyle(lt));

	painter.setPen(pen);
	painter.setBrush(Qt::NoBrush);
	painter.drawPolygon(qpt, count);

	DeleteQPoints(qpt);
}

void QtGeoSurface::FillPolygon(int count, int * surfX, int * surfY, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int ft)
{
	EnsurePaintSurfaceValid();
	if (_osSurf4Paint == nullptr)
		return;

	QPoint* qpt = CreateQPoints(count, surfX, surfX);

	QPainter painter(_osSurf4Paint);
	QBrush brush(QColor(r, g, b, a), TranslateFillStyle(ft));
	painter.setBrush(brush);
	painter.setPen(Qt::NoPen);
	painter.drawPolygon(qpt, count);

	DeleteQPoints(qpt);
}

void QtGeoSurface::DrawEllipse(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt)
{
	EnsurePaintSurfaceValid();
	if (_osSurf4Paint == nullptr)
		return;

	QPainter painter(_osSurf4Paint);
	QPen pen(QColor(r, g, b, a), lw, TranslateLineStyle(lt));
	painter.setPen(pen);
	painter.setBrush(Qt::NoBrush);
	painter.drawEllipse(surfX, surfY, width, height);
}

void QtGeoSurface::FillEllipse(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int ft)
{
	EnsurePaintSurfaceValid();
	if (_osSurf4Paint == nullptr)
		return;

	QPainter painter(_osSurf4Paint);
	QBrush brush(QColor(r, g, b, a),TranslateFillStyle(ft));
	painter.setBrush(brush);
	painter.setPen(Qt::NoPen);
	painter.drawEllipse(surfX, surfY, width, height);
}

void QtGeoSurface::DrawRect(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt)
{
	EnsurePaintSurfaceValid();
	if (_osSurf4Paint == nullptr)
		return;

	QPainter painter(_osSurf4Paint);
	QPen pen(QColor(r, g, b, a), lw, TranslateLineStyle(lt));
	painter.setPen(pen);
	painter.setBrush(Qt::NoBrush);
	painter.drawRect(surfX, surfY, width, height);
}

void QtGeoSurface::FillRect(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int ft)
{
	EnsurePaintSurfaceValid();
	if (_osSurf4Paint == nullptr)
		return;

	QPainter painter(_osSurf4Paint);
	QBrush brush(QColor(r, g, b, a), TranslateFillStyle(ft));
	painter.setPen(Qt::NoPen);
	painter.fillRect(surfX, surfY, width, height, brush);
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
