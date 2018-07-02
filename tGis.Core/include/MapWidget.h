#pragma once

#ifndef __MAPWIDGET_H__
#define __MAPWIDGET_H__

#include "Helper.h"

#include "IMapWidget.h"
#include "OverlayLayer.h"

#include <vector>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

class TGIS_API MapWidget : public IMapWidget
{
public:
	MapWidget();
	virtual ~MapWidget();

	virtual IMap* GetMap() = 0;
	virtual IGeoSurface* GetGeoSurface() = 0;
	virtual IOverlayLayer* GetOverlayLayer();

	virtual bool AddMapTool(IMapTool*);
	virtual bool RemoveMapTool(IMapTool*);
	virtual void SetBackgroundColor(unsigned char R, unsigned char G, unsigned char B);
	virtual void GetBackgroundColor(unsigned char* R, unsigned char* G, unsigned char* B);
	virtual void SetGridVisible(bool);
	virtual bool GetGridVisible();
	virtual void RepaintMap();

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
	OverlayLayer _overlayLayer;
	vector<IMapTool*> _vecMapTool;
	unsigned char _backgroundR;
	unsigned char _backgroundG;
	unsigned char _backgroundB;
	bool _gridVisible;
};


END_NAME_SPACE(tGis, Core)


#endif


