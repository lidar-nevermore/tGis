#pragma once

#ifndef __MAPWIDGET_H__
#define __MAPWIDGET_H__

#include "Helper.h"

#include "IMapWidget.h"
#include "OverlayLayer.h"

#include <vector>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)


class TGISCORE_API MapWidget : public IMapWidget
{
public:
	MapWidget();
	virtual ~MapWidget();

	virtual IMap* GetMap() = 0;
	virtual IGeoSurface* GetGeoSurface() = 0;
	virtual IOverlayLayer* GetOverlayLayer();

	virtual bool AddMapTool(IMapTool*);
	virtual void RemoveMapTool(IMapTool*);
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
};


END_NAME_SPACE(tGis, Core)


#endif


