#pragma once

#ifndef __MAPWIDGET_H__
#define __MAPWIDGET_H__

#include "Helper.h"

#include "IMapWidget.h"
#include "OverlayLayer.h"

#include <vector>

using namespace std;



BEGIN_NAME_SPACE(tGis, Core)

class GeoSurface;

class TGIS_API MapWidget : public IMapWidget
{
public:
	MapWidget(GeoSurface* geoSurface);
	virtual ~MapWidget();

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

	virtual void MouseDown(void*);
	virtual void MouseMove(void*);
	virtual void MouseUp(void*);
	virtual void MouseClicked(void*);
	virtual void MouseDoubleClicked(void*);
	virtual void MouseWheel(void*);

	virtual void KeyDown(void*);
	virtual void KeyUp(void*);
	virtual void KeyPress(void*);

protected:
	void SetSurfaceSize(int surfW, int surfH)
	{
		_viewPort.SetSurfaceSize(surfW, surfH);
	}

protected:
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


