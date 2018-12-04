#include "QMapWidget.h"
#include <QDesktopWidget>
#include <QApplication>
#include "qevent.h"

#include "tVisualize.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Utility)

QMapWidget::QMapWidget(QWidget *parent)
	:QWidget(parent)
	,MapWidget(&_geoSurface)
{
	_firstResizing = true;
	_map = nullptr;
	_backgroundR = 255;
	_backgroundG = 255;
	_backgroundB = 255;
}


QMapWidget::~QMapWidget()
{
}

void QMapWidget::Client2Screen(int cliX, int cliY, int * scrX, int * scrY)
{
	QPoint pt(cliX, cliY);
	QPoint gpt = mapToGlobal(pt);
	if (scrX != nullptr)
		*scrX = gpt.x();
	if (scrY != nullptr)
		*scrY = gpt.y();
}

void QMapWidget::Screen2Client(int scrX, int scrY, int * cliX, int * cliY)
{
	QPoint pt(scrX, scrY);
	QPoint gpt = mapFromGlobal(pt);
	if (cliX != nullptr)
		*cliX = gpt.x();
	if (cliY != nullptr)
		*cliY = gpt.y();
}

void QMapWidget::SetMap(IMap *map)
{
	_map = map;
}

IMap * QMapWidget::GetMap()
{
	return _map;
}

void QMapWidget::SetBackgroundColor(unsigned char R, unsigned char G, unsigned char B)
{
	_backgroundR = R;
	_backgroundG = G;
	_backgroundB = B;
}

void QMapWidget::RepaintMap()
{
	MapWidget::RepaintMap();
	update();
}

void QMapWidget::PresentMap()
{
	MapWidget::PresentMap();
	update();
}

void QMapWidget::paintEvent(QPaintEvent *)
{
	QSize sz = size();
	{
		QPainter painter(this);
		painter.fillRect(0, 0, sz.width(), sz.height(), QColor(_backgroundR, _backgroundG, _backgroundB));
	}
	PresentMap();
	QPainter painter(this);
	_geoSurface.AttachQPainter(&painter);
	_geoSurface.BeginPaintOnAttachedQPainter();
	_overlayLayer.Paint((IGeoSurface*)&_geoSurface);
	_geoSurface.EndPaintOnAttachedQPainter();
	_geoSurface.DetachQPainter();
	if (_gridVisible)
	{
		QPen pen(QColor(225, 35, 225, 255), 1, Qt::DashLine);
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
	
	SetSurfaceSize(width, height);

	QDesktopWidget * desktop = QApplication::desktop();
	int curMonitor = desktop->screenNumber(this);
	QRect rect = desktop->screenGeometry(curMonitor);
	SetMaxSurfaceSize(rect.width(), rect.height());

	IMapWidget* mapWidget = this;
	if (_firstResizing)
	{
		_firstResizing = false;
		LoadedEvent(mapWidget, width, height);
	}
	SizeChangedEvent(mapWidget, width, height);
	RepaintMap();
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

