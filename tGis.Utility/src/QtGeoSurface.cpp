#include "QtGeoSurface.h"
#include "SimpleLineSymbol.h"
#include "SimpleFillSymbol.h"
#include "QMapWidget.h"

BEGIN_NAME_SPACE(tGis, Utility)


QtGeoSurface::QtGeoSurface()
{
	_paintOnAttachedQPainter = false;
	_attachedPainter = nullptr;
	_osSurf4Paint = nullptr;
}


QtGeoSurface::~QtGeoSurface()
{
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
	int surfWidth;
	int surfHeight;
	_viewPort.GetSurfaceSize(&surfWidth, &surfHeight);
	if (surfWidth > psw && surfHeight > psh)
	{
		if (_osSurf4Paint != nullptr)
			delete _osSurf4Paint;
		_osSurf4Paint = new QPixmap(surfWidth, surfHeight);
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
	_attachedPainter = painter;
}

void QtGeoSurface::DetachQPainter()
{
	_attachedPainter = nullptr;
}

void QtGeoSurface::BeginPaintOnAttachedQPainter()
{
	_paintOnAttachedQPainter = true;
}

void QtGeoSurface::EndPaintOnAttachedQPainter()
{
	_paintOnAttachedQPainter = false;
}

void QtGeoSurface::Present(IWidget * w, int wX, int wY)
{
	if (_osSurf4Paint == nullptr)
		return;
	QMapWidget* widget = (QMapWidget*)w;
	QPainter painter(widget);
	painter.drawPixmap(wX, wY, *_osSurf4Paint);
}

void QtGeoSurface::Present(IWidget * w, int wX, int wY, int wW, int wH)
{
	if (_osSurf4Paint == nullptr)
		return;
	QMapWidget* widget = (QMapWidget*)w;
	QPainter painter(widget);
	painter.drawPixmap(wX, wY, wW, wH, *_osSurf4Paint);
}

void QtGeoSurface::Present(IWidget * w, int wX, int wY, int wW, int wH, int surfX, int surfY, int surfW, int surfH)
{
	if (_osSurf4Paint == nullptr)
		return;
	QMapWidget* widget = (QMapWidget*)w;
	QPainter painter(widget);
	painter.drawPixmap(wX, wY, wW, wH, *_osSurf4Paint, surfX, surfY, surfW, surfH);
}

class QPainterPtrFreeHelper
{
public:
	QPainterPtrFreeHelper()
	{
		_autoDelte = false;
	}

	~QPainterPtrFreeHelper()
	{
		if (_autoDelte)
			delete _attachedPainter;
	}

	operator QPainter&() {
		return *_attachedPainter;
	}

	QPainter& operator*() {
		return *_attachedPainter;
	}

	QPainterPtrFreeHelper &operator=(QPainter* painter)
	{
		_attachedPainter = painter;
		return *this;
	}

private:
	QPainterPtrFreeHelper(const QPainterPtrFreeHelper &) = delete;
	QPainterPtrFreeHelper &operator=(const QPainterPtrFreeHelper &) = delete;

public:
	bool _autoDelte;
	QPainter* _attachedPainter;
};

void QtGeoSurface::DrawPolyline(int count, int * surfX, int * surfY, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt)
{
	QPainterPtrFreeHelper painter_ptr;

	if (_paintOnAttachedQPainter)
	{
		if (_attachedPainter == nullptr)
			return;

		painter_ptr = _attachedPainter;
	}
	else
	{
		EnsurePaintSurfaceValid();
		if (_osSurf4Paint == nullptr)
			return;
		painter_ptr = new QPainter(_osSurf4Paint);
		painter_ptr._autoDelte = true;

	}

	QPoint* qpt = CreateQPoints(count, surfX, surfY);
	QPainter& painter = painter_ptr;
	QPen pen(QColor(r, g, b, a), lw, TranslateLineStyle(lt));

	painter.setPen(pen);
	painter.setBrush(Qt::NoBrush);
	painter.drawPolyline(qpt, count);

	DeleteQPoints(qpt);
}

void QtGeoSurface::DrawPolygon(int count, int * surfX, int * surfY, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt)
{
	QPainterPtrFreeHelper painter_ptr;

	if (_paintOnAttachedQPainter)
	{
		if (_attachedPainter == nullptr)
			return;

		painter_ptr = _attachedPainter;
	}
	else
	{
		EnsurePaintSurfaceValid();
		if (_osSurf4Paint == nullptr)
			return;
		painter_ptr = new QPainter(_osSurf4Paint);
		painter_ptr._autoDelte = true;

	}

	QPoint* qpt = CreateQPoints(count, surfX, surfY);
	QPainter& painter = painter_ptr;
	QPen pen(QColor(r, g, b, a), lw, TranslateLineStyle(lt));

	painter.setPen(pen);
	painter.setBrush(Qt::NoBrush);
	painter.drawPolygon(qpt, count);

	DeleteQPoints(qpt);
}

void QtGeoSurface::FillPolygon(int count, int * surfX, int * surfY, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int ft)
{
	QPainterPtrFreeHelper painter_ptr;

	if (_paintOnAttachedQPainter)
	{
		if (_attachedPainter == nullptr)
			return;

		painter_ptr = _attachedPainter;
	}
	else
	{
		EnsurePaintSurfaceValid();
		if (_osSurf4Paint == nullptr)
			return;
		painter_ptr = new QPainter(_osSurf4Paint);
		painter_ptr._autoDelte = true;

	}

	QPoint* qpt = CreateQPoints(count, surfX, surfY);
	QPainter& painter = painter_ptr;
	QBrush brush(QColor(r, g, b, a), TranslateFillStyle(ft));
	painter.setBrush(brush);
	painter.setPen(Qt::NoPen);
	painter.drawPolygon(qpt, count);

	DeleteQPoints(qpt);
}

void QtGeoSurface::DrawEllipse(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt)
{
	QPainterPtrFreeHelper painter_ptr;

	if (_paintOnAttachedQPainter)
	{
		if (_attachedPainter == nullptr)
			return;

		painter_ptr = _attachedPainter;
	}
	else
	{
		EnsurePaintSurfaceValid();
		if (_osSurf4Paint == nullptr)
			return;
		painter_ptr = new QPainter(_osSurf4Paint);
		painter_ptr._autoDelte = true;

	}

	QPainter& painter = painter_ptr;
	QPen pen(QColor(r, g, b, a), lw, TranslateLineStyle(lt));
	painter.setPen(pen);
	painter.setBrush(Qt::NoBrush);
	painter.drawEllipse(surfX, surfY, width, height);
}

void QtGeoSurface::FillEllipse(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int ft)
{
	QPainterPtrFreeHelper painter_ptr;

	if (_paintOnAttachedQPainter)
	{
		if (_attachedPainter == nullptr)
			return;

		painter_ptr = _attachedPainter;
	}
	else
	{
		EnsurePaintSurfaceValid();
		if (_osSurf4Paint == nullptr)
			return;
		painter_ptr = new QPainter(_osSurf4Paint);
		painter_ptr._autoDelte = true;

	}

	QPainter& painter = painter_ptr;
	QBrush brush(QColor(r, g, b, a),TranslateFillStyle(ft));
	painter.setBrush(brush);
	painter.setPen(Qt::NoPen);
	painter.drawEllipse(surfX, surfY, width, height);
}

void QtGeoSurface::DrawRect(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int lw, int lt)
{
	QPainterPtrFreeHelper painter_ptr;

	if (_paintOnAttachedQPainter)
	{
		if (_attachedPainter == nullptr)
			return;

		painter_ptr = _attachedPainter;
	}
	else
	{
		EnsurePaintSurfaceValid();
		if (_osSurf4Paint == nullptr)
			return;
		painter_ptr = new QPainter(_osSurf4Paint);
		painter_ptr._autoDelte = true;

	}

	QPainter& painter = painter_ptr;
	QPen pen(QColor(r, g, b, a), lw, TranslateLineStyle(lt));
	painter.setPen(pen);
	painter.setBrush(Qt::NoBrush);
	painter.drawRect(surfX, surfY, width, height);
}

void QtGeoSurface::FillRect(int surfX, int surfY, int width, int height, unsigned char r, unsigned char g, unsigned char b, unsigned char a, int ft)
{
	QPainterPtrFreeHelper painter_ptr;

	if (_paintOnAttachedQPainter)
	{
		if (_attachedPainter == nullptr)
			return;

		painter_ptr = _attachedPainter;
	}
	else
	{
		EnsurePaintSurfaceValid();
		if (_osSurf4Paint == nullptr)
			return;
		painter_ptr = new QPainter(_osSurf4Paint);
		painter_ptr._autoDelte = true;

	}

	QPainter& painter = painter_ptr;
	QBrush brush(QColor(r, g, b, a), TranslateFillStyle(ft));
	painter.setPen(Qt::NoPen);
	painter.fillRect(surfX, surfY, width, height, brush);
}

void QtGeoSurface::DrawImage(const unsigned char * buf, int surfX, int surfY, int width, int height)
{
	QPainterPtrFreeHelper painter_ptr;

	if (_paintOnAttachedQPainter)
	{
		if (_attachedPainter == nullptr)
			return;

		painter_ptr = _attachedPainter;
	}
	else
	{
		EnsurePaintSurfaceValid();
		if (_osSurf4Paint == nullptr)
			return;
		painter_ptr = new QPainter(_osSurf4Paint);
		painter_ptr._autoDelte = true;

	}

	QPainter& painter = painter_ptr;
	QImage img(buf, width, height, QImage::Format_ARGB32);
	painter.drawImage(QRect(surfX, surfY, width, height), img, QRect(0, 0, width, height), Qt::ColorOnly|Qt::NoOpaqueDetection|Qt::AvoidDither);
}


END_NAME_SPACE(tGis, Utility)
