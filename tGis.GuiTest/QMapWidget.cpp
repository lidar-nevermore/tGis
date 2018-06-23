#include "QMapWidget.h"
#include "qevent.h"


QMapWidget::QMapWidget(QWidget *parent)
	:QWidget(parent)
	,_map()
{
	_firstTime = true;
	_surfBackgroundR = 255;
	_surfBackgroundG = 255;
	_surfBackgroundB = 255;

	_geoSurface.SetBackgroundColor(255, 255, 255);
	

	this->AddMapTool(&_mapPanTool);
	this->AddMapTool(&_mapZoomTool);
}


QMapWidget::~QMapWidget()
{
}

void QMapWidget::SetMap(IMap *)
{
}

IMap * QMapWidget::GetMap()
{
	return &_map;
}

IGeoSurface * QMapWidget::GetGeoSurface()
{
	return (IGeoSurface*)&_geoSurface;
}

IOverlayLayer * QMapWidget::GetScreenLayer()
{
	return nullptr;
}

void QMapWidget::SetBackgroundColor(unsigned char R, unsigned char G, unsigned char B)
{
	_surfBackgroundR = R;
	_surfBackgroundG = G;
	_surfBackgroundB = B;
	_geoSurface.SetBackgroundColor(R, G, B);
}

void QMapWidget::RepaintMap()
{
	MapWidget::RepaintMap();
	update();
}

void QMapWidget::PresentSurface()
{
	update();
}

void QMapWidget::paintEvent(QPaintEvent *)
{
	QSize sz = size();
	QPainter painter(this);
	painter.fillRect(0, 0, sz.width(), sz.height(), QColor(_surfBackgroundR, _surfBackgroundG, _surfBackgroundB));
	_geoSurface.AttachQPainter(&painter);
	_geoSurface.PresentSurface();
	_geoSurface.DetachQPainter();
}

void QMapWidget::resizeEvent(QResizeEvent * e)
{
	QSize sz = e->size();
	_geoSurface.SetViewSize(sz.width(), sz.height());
	if (_firstTime)
	{
		_firstTime = false;
		//const OGREnvelope* envelope = _vecLayer.GetEnvelope();
		//_geoSurface.IncludeEnvelope(envelope);
	}
	MapWidget::RepaintMap();
}

void QMapWidget::mousePressEvent(QMouseEvent *e)
{
	MapWidget::MouseDown(e);
}

void QMapWidget::mouseReleaseEvent(QMouseEvent *e)
{
	MapWidget::MouseUp(e);
}

void QMapWidget::mouseMoveEvent(QMouseEvent *e)
{
	MapWidget::MouseMove(e);
}

void QMapWidget::wheelEvent(QWheelEvent * e)
{
	MapWidget::MouseWheel(e);
}
