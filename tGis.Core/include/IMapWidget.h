#pragma once

#ifndef __I_MAPWIDGET_H__
#define __I_MAPWIDGET_H__

#include "Helper.h"
#include "Event.h"
#include "IWidget.h"
#include "GeoViewPort.h"

BEGIN_NAME_SPACE(tGis, Core)


struct IMap;
struct IGeoSurface;
struct IMapTool;
struct IOverlayLayer;
struct IMapWidget;

template struct TGIS_CORE_API IEventHandler<IMapWidget*, int, int>;
template class TGIS_CORE_API Event<IMapWidget*, int, int>;

template struct TGIS_CORE_API IEventHandler<IMapWidget*, IMapTool*>;
template class TGIS_CORE_API Event<IMapWidget*, IMapTool*>;

struct TGIS_CORE_API IMapWidget : public IWidget
{
	virtual void SetMap(IMap*) = 0;
	virtual IMap* GetMap() = 0;
	virtual GeoViewPort* GetViewPort() = 0;
	virtual IOverlayLayer* GetOverlayLayer() = 0;

	virtual bool AddMapTool(IMapTool*) = 0;
	virtual bool RemoveMapTool(IMapTool*) = 0;
	virtual void SetBackgroundColor(unsigned char R, unsigned char G, unsigned char B) = 0;
	virtual void GetBackgroundColor(unsigned char* R, unsigned char* G, unsigned char* B) = 0;
	virtual void SetGridVisible(bool) = 0;
	virtual bool GetGridVisible() = 0;

	virtual void RepaintMap() = 0;
	virtual void PresentMap() = 0;

	Event<IMapWidget*, int, int> LoadedEvent;
	Event<IMapWidget*, int, int> SizeChangedEvent;
	Event<IMapWidget*, IMapTool*> MapToolChangedEvent;
	Event<IMapWidget*, IMapTool*> MapToolAddedEvent;
	Event<IMapWidget*, IMapTool*> MapToolRemovedEvent;

	IMapWidget() {};
	virtual ~IMapWidget() {};
private:
	IMapWidget(const IMapWidget &) = delete;
	IMapWidget &operator=(const IMapWidget &) = delete;
};

typedef IMapWidget* IMapWidgetPtr;

END_NAME_SPACE(tGis, Core)


#endif
