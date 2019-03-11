#pragma once

#ifndef __TGISAPPLICATION_H__
#define __TGISAPPLICATION_H__


#include "Helper.h"
#include "tGisCoreCfg.h"
#include "tEntity.h"
#include "tVisualize.h"
#include "Event.h"
#include "ilog.h"


BEGIN_NAME_SPACE(tGis, Core)

template struct TGIS_CORE_API IEventHandler<const char*, const char*, const char*, const char*>;
template class TGIS_CORE_API Event<const char*, const char*, const char*, const char*>;

#define TGIS_LOG_FORMAT(level, format, ...) print_log(TGisApplication::INSTANCE().GetLogger(), level, format, ##__VA_ARGS__)

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

private:
	void* _loger;
	char _exeDir[TGIS_MAX_PATH];

public:
	virtual IMap*  GetCurrentMap() = 0;
	virtual IMapWidget*  GetCurrentMapWidget() = 0;
	const char* GetExeDir() { return _exeDir; }
	void* GetLogger() { return _loger; }
};


END_NAME_SPACE(tGis, Core)


#endif

