#pragma once

#ifndef __PLUGINMANAGER_H__
#define __PLUGINMANAGER_H__

#include "Helper.h"
#include "Event.h"
#include "ITGisObject.h"
#include <vector>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

class Plugin;
template struct TGIS_API IEventHandler<Plugin*,const char*>;
template class TGIS_API Event<Plugin*,const char*>;

class TGIS_API PluginManager
{
private:
	static PluginManager* _instance;
public:
	static PluginManager& INSTANCE();

private:
	PluginManager();
public:
	~PluginManager();

private:
	vector<Plugin*> _plugins;

public:
	Event<Plugin*, const char*> LoadPluginEvent;
	void LoadPlugins();
};


END_NAME_SPACE(tGis, Core)


#endif
