#pragma once

#ifndef __PLUGINMANAGER_H__
#define __PLUGINMANAGER_H__

#include "Helper.h"
#include "Event.h"
#include "ITGisObject.h"
#include <vector>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

class Plugin;

class TGIS_CORE_API PluginManager
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
	string _pluginsPath;
	vector<Plugin*> _plugins;

public:
	void LoadPlugins();
	const char* GetPluginsPath() { return _pluginsPath.c_str(); }
};


END_NAME_SPACE(tGis, Core)


#endif
