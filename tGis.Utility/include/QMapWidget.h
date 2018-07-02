#pragma once

#ifndef __QMAPWIDGET_H__
#define __QMAPWIDGET_H__

#ifdef TGIS_UTILITY_EXPORTS
#include "QtHelper.h"
#endif

#include <QWidget>
#include <QPainter>

#include "tVisualize.h"
#include "QtGeoSurface.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Utility)

class TGIS_UTILITY_API QMapWidget : public QWidget,public MapWidget
{
	Q_OBJECT
public:
	explicit QMapWidget(QWidget *parent = 0);
	~QMapWidget();

public:
	void SetMap(IMap*);
	IMap* GetMap();
	IGeoSurface* GetGeoSurface();
	IOverlayLayer* GetScreenLayer();
	void SetBackgroundColor(unsigned char R, unsigned char G, unsigned char B);
	void RepaintMap();
	void PresentMap();

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


END_NAME_SPACE(tGis, Utility)


#endif
