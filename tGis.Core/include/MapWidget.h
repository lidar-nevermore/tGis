#pragma once

#ifndef __MAPWIDGET_H__
#define __MAPWIDGET_H__

#include "Helper.h"

#include "IMapWidget.h"
#include "OverlayLayer.h"
#include "IMap.h"
#include "LayerRender.h"

#include <vector>

using namespace std;



BEGIN_NAME_SPACE(tGis, Core)

class GeoSurface;

class TGIS_CORE_API MapWidget : public IMapWidget
{
public:
	MapWidget();
	virtual ~MapWidget();

	virtual void SetMap(IMap*);
	virtual IMap* GetMap()
	{
		return _map;
	}

	virtual GeoViewPort* GetViewPort() 
	{
		return &_viewPort;
	};
	virtual IOverlayLayer* GetOverlayLayer()
	{
		return &_overlayLayer;
	};

	virtual bool AddMapTool(IMapTool*);
	virtual bool RemoveMapTool(IMapTool*);
	virtual void SetBackgroundColor(unsigned char R, unsigned char G, unsigned char B);
	virtual void GetBackgroundColor(unsigned char* R, unsigned char* G, unsigned char* B);
	virtual void SetGridVisible(bool);
	virtual bool GetGridVisible();

	virtual void RepaintMap();
	virtual void PresentMap();

public:
	inline static void SetMaxSize(int width, int height)
	{
		LayerRender::SetMaxSurfaceSize(width, height);
	}

protected:
	inline void SetSurfaceSize(int surfW, int surfH)
	{
		_viewPort.SetSurfaceSize(surfW, surfH);
	}

	void LayerAdded(IMapPtr, ILayerPtr);
	void LayerRemoved(IMapPtr, ILayerPtr);
	void LayerCleared(IMapPtr);

protected:
	IMap* _map;
	//�����MapWidgetʵ���ṩ�����GeoSurface,
	//Ȼ����û�����_geoSurfaceָ�븳ֵ
	GeoSurface* _geoSurface;
	GeoViewPort _viewPort;
	OverlayLayer _overlayLayer;
	vector<IMapTool*> _vecMapTool;
	unsigned char _backgroundR;
	unsigned char _backgroundG;
	unsigned char _backgroundB;
	bool _gridVisible;
};


END_NAME_SPACE(tGis, Core)


#endif


