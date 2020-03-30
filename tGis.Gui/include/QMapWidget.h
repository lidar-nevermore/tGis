#pragma once

#ifndef __QMAPWIDGET_H__
#define __QMAPWIDGET_H__

#include <QWidget>
#include <QPainter>

#include "tGisGuiCfg.h"

#include "tVisualize.h"
#include "QtGeoSurface.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

template struct TGIS_GUI_API IEventHandler<QMapWidget*, QMouseEvent*>;
template class TGIS_GUI_API Event<QMapWidget*, QMouseEvent*>;

template struct TGIS_GUI_API IEventHandler<QMapWidget*, QWheelEvent*>;
template class TGIS_GUI_API Event<QMapWidget*, QWheelEvent*>;

class TGIS_GUI_API QMapWidget : public QWidget,public MapWidget
{
	friend class QtGeoSurface;
	Q_OBJECT
public:
	explicit QMapWidget(QWidget *parent = 0);
	~QMapWidget();

public:
	void Client2Screen(int cliX, int cliY, int *scrX, int *scrY);
	void Screen2Client(int scrX, int scrY, int *cliX, int *cliY);

public:
	void SetMap(IMap*);
	IMap* GetMap();

	void SetBackgroundColor(unsigned char R, unsigned char G, unsigned char B);

	void RepaintMap();
	void PresentMap();

public:
	Event<QMapWidget*, QMouseEvent*> MousePressEvent;
	Event<QMapWidget*, QMouseEvent*> MouseReleaseEvent;
	Event<QMapWidget*, QMouseEvent*> MouseMoveEvent;
	Event<QMapWidget*, QWheelEvent*> WheelEvent;

private:
	QtGeoSurface _geoSurface;
	IMap* _map;
	bool _firstResizing;

protected:
	virtual void paintEvent(QPaintEvent *);

	virtual void resizeEvent(QResizeEvent *);

	virtual void mousePressEvent(QMouseEvent *);
	virtual void mouseReleaseEvent(QMouseEvent *);
	virtual void mouseMoveEvent(QMouseEvent *);
	virtual void wheelEvent(QWheelEvent *);
};


END_NAME_SPACE(tGis, Gui)


#endif
