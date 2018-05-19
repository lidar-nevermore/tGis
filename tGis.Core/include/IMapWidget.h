#pragma once

#ifndef __I_MAPWIDGET_H__
#define __I_MAPWIDGET_H__

#include "Helper.h"


BEGIN_NAME_SPACE(tGis, Core)


struct IMap;
struct IGeoSurface;
struct IMapTool;
struct IOverlayLayer;

struct TGISCORE_API IMapWidget
{
	virtual void SetMap(IMap*) = 0;
	virtual IMap* GetMap() = 0;
	virtual IGeoSurface* GetGeoSurface() = 0;
	virtual IOverlayLayer* GetOverlayLayer() = 0;

	virtual bool AddMapTool(IMapTool*) = 0;
	virtual void RemoveMapTool(IMapTool*) = 0;
	virtual void SetBackgroundColor(unsigned char R, unsigned char G, unsigned char B) = 0;

	virtual void RepaintMap() = 0;
	virtual void PresentSurface() = 0;

	virtual void MouseDown(void*) = 0;
	virtual void MouseMove(void*) = 0;
	virtual void MouseUp(void*) = 0;
	virtual void MouseClicked(void*) = 0;
	virtual void MouseDoubleClicked(void*) = 0;
	virtual void MouseWheel(void*) = 0;

	virtual void KeyDown(void*) = 0;
	virtual void KeyUp(void*) = 0;
	virtual void KeyPress(void*) = 0;

	IMapWidget() {};
	virtual ~IMapWidget() {};
private:
	IMapWidget(const IMapWidget &) = delete;
	IMapWidget &operator=(const IMapWidget &) = delete;
};


END_NAME_SPACE(tGis, Core)


#endif
