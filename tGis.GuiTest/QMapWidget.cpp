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

	_dataset.Attach("E:\\杂项\\9.jpg", GA_ReadOnly);
	_layer.SetDataset(&_dataset, 1, 2, 3);

	_vector.Attach("E:\\SpatialData\\全国省市县\\BOUND_A省.shp", GA_ReadOnly);
	_vecLayer.SetOGRLayer(&_vector,_vector.GetGDALDataset()->GetLayer(0),-1);
	//_map.AddLayer(&_vecLayer);



	const OGREnvelope* envelope = _layer.GetEnvelope();
	_geoSurface.SetSpatialReference(_layer.GetSpatialReference());
	_geoSurface.SetViewResolution(0.9);
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
		const OGREnvelope* envelope = _layer.GetEnvelope();
		_geoSurface.IncludeEnvelope(envelope);
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
