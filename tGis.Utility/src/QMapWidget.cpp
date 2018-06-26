#include "QMapWidget.h"
#include <QDesktopWidget>
#include <QApplication>
#include "qevent.h"

#include "tVisualize.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Utility)

QMapWidget::QMapWidget(QWidget *parent)
	:QWidget(parent)
{
	_firstResizing = true;
	_map = nullptr;
	_geoSurface.SetBackgroundColor(_backgroundR, _backgroundG, _backgroundB);
}


QMapWidget::~QMapWidget()
{
}

void QMapWidget::SetMap(IMap *map)
{
	_map = map;
}

IMap * QMapWidget::GetMap()
{
	return _map;
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
	_backgroundR = R;
	_backgroundG = G;
	_backgroundB = B;
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
	painter.fillRect(0, 0, sz.width(), sz.height(), QColor(_backgroundR, _backgroundG, _backgroundB));
	_geoSurface.AttachQPainter(&painter);
	_geoSurface.PresentSurface();
	_geoSurface.BeginPaintOnAttachedQPainter();
	_overlayLayer.Paint((IGeoSurface*)&_geoSurface);
	_geoSurface.EndPaintOnAttachedQPainter();
	_geoSurface.DetachQPainter();
	if (_gridVisible)
	{
		QPen pen(QColor(30, 30, 30, 255), 1, Qt::DashLine);
		painter.setPen(pen);
		painter.setBrush(Qt::NoBrush);
		for (int x = 25; x < sz.width(); x += 25)
		{
			painter.drawLine(x, 0, x, sz.height());
		}
		for (int y = 25; y < sz.height(); y += 25)
		{
			painter.drawLine(0, y, sz.width(), y);
		}
	}
}

void QMapWidget::resizeEvent(QResizeEvent * e)
{
	QSize sz = e->size();
	int width = sz.width();
	int height = sz.height();
	IMapWidget* mapWidget = this;

	_geoSurface.SetViewSize(width, height);
	QDesktopWidget * desktop = QApplication::desktop();
	int curMonitor = desktop->screenNumber(this);
	QRect rect = desktop->screenGeometry(curMonitor);
	SetMaxSurfaceSize(rect.width(), rect.height());
	if (_firstResizing)
	{
		_firstResizing = false;
		LoadedEvent(mapWidget, width, height);
	}
	SizeChangedEvent(mapWidget, width, height);
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



END_NAME_SPACE(tGis, Utility)

