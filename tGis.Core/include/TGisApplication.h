#pragma once

#ifndef __TGISAPPLICATION_H__
#define __TGISAPPLICATION_H__


#include "Helper.h"
#include "tGisCoreCfg.h"
#include "tEntity.h"
#include "tVisualize.h"
#include "Event.h"


BEGIN_NAME_SPACE(tGis, Core)

template struct TGIS_CORE_API IEventHandler<const char*, const char*, const char*, const char*>;
template class TGIS_CORE_API Event<const char*, const char*, const char*, const char*>;

class TGIS_CORE_API TGisApplication
{
private:
	static TGisApplication* _instance;
public:
	static TGisApplication& INSTANCE();
	static Event<const char*, const char*, const char*, const char*> LoadPluginEvent;

protected:
	TGisApplication();
	virtual ~TGisApplication();

public:
	virtual IMap*  GetCurrentMap() = 0;
	virtual IMapWidget*  GetCurrentMapWidget() = 0;
};


END_NAME_SPACE(tGis, Core)


#endif

