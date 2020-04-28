#pragma once

#ifndef __TGISAPPLICATION_H__
#define __TGISAPPLICATION_H__


#include "Helper.h"
#include "tGis_Entity.h"
#include "tGis_Visualize.h"
#include "Event.h"
#include "ilog.h"


BEGIN_NAME_SPACE(tGis, Core)

template struct TGIS_CORE_API IEventHandler<const char*, const char*, const char*, const char*>;
template class TGIS_CORE_API Event<const char*, const char*, const char*, const char*>;

#define TGIS_LOG_FORMAT(level, format, ...) print_log(tGisApplication::INSTANCE()->GetLogger(), level, format, ##__VA_ARGS__)

class TGIS_CORE_API tGisApplication
{
private:
	static tGisApplication* _instance;
public:
	static tGisApplication* INSTANCE();
	static Event<const char*, const char*, const char*, const char*> LoadPluginEvent;

protected:
	tGisApplication();
	virtual ~tGisApplication();

private:
	void* _loger;
	char _exeDir[TGIS_MAX_PATH];

public:
	const char* GetExeDir() { return _exeDir; }
	void* GetLogger() { return _loger; }
};


END_NAME_SPACE(tGis, Core)


#endif

