#pragma once

#include <QWidget>
#include <QPainter>

#include "tUtility.h"
#include "QtGeoSurface.h"
#include "MapPanTool.h"

#include "MapZoomTool.h"

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
	MyGDALRasterDataset _dataset;
	RasterGrayScaleLayer _layer;

	MyGDALVectorDataset _vector;
	VectorSimpleLayer _vecLayer;

	bool _firstTime;

	Map _map;
	MapPanTool _mapPanTool;
	MapZoomTool _mapZoomTool;

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

