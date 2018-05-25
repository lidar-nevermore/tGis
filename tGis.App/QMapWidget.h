#pragma once

#include <QWidget>
#include <QPainter>

#include "tVisualize.h"
#include "QtGeoSurface.h"

using namespace tGis::Core;


class QMapWidget : public QWidget,public MapWidget
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
	void PresentSurface();

private:
	QtGeoSurface _geoSurface;
	IMap* _map;

	unsigned char _surfBackgroundR;
	unsigned char _surfBackgroundG;
	unsigned char _surfBackgroundB;

protected:
	void paintEvent(QPaintEvent *);

	void resizeEvent(QResizeEvent *);

	void mousePressEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void wheelEvent(QWheelEvent *);
};

