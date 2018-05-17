#include "QMapWidget.h"
#include "qevent.h"
#include "tUtility.h"

QMapWidget::QMapWidget(QWidget *parent)
	:QWidget(parent)
	,_map()
{
	_surfBackgroundR = 255;
	_surfBackgroundG = 255;
	_surfBackgroundB = 255;

	_dataset.Attach("E:\\imLUM2.png",GA_ReadOnly);
	_layer.SetDataset(&_dataset, 1);
	const OGREnvelope* envelope = _dataset.GetEnvelope();
	_geoSurface.SetSpatialReference(_dataset.GetSpatialReference());
	double resx, resy;
	_dataset.Pixel2Spatial(1, 1, &resx, &resy);
	_geoSurface.SetViewResolution((resx- envelope->MinX)/1.7);
	_geoSurface.SetViewCenter((envelope->MinX + envelope->MaxX) / 2, (envelope->MinY + envelope->MaxY) / 2);
	_geoSurface.SetBackgroundColor(255, 255, 255);
	_map.AddLayer(&_layer);
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

IScreenLayer * QMapWidget::GetScreenLayer()
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
	repaint();
}

void QMapWidget::PresentSurface()
{
	repaint();
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
