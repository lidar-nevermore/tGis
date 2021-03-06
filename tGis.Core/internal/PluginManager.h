#pragma once

#ifndef __PLUGINMANAGER_H__
#define __PLUGINMANAGER_H__

#include "Helper.h"
#include "Event.h"
#include "ITypedObject.h"
#include <vector>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

class Plugin;

class PluginManager
{
private:
	static PluginManager* _instance;
public:
	static PluginManager* INSTANCE();

private:
	PluginManager();
public:
	~PluginManager();

private:
	string _pluginsPath;
	vector<Plugin*> _plugins;

private:
	static void OnTraverseDir(void* usr, const char * dir, const char* name, unsigned int attrib);

public:
	void LoadPlugins();
	const char* GetPluginsPath() { return _pluginsPath.c_str(); }
};


END_NAME_SPACE(tGis, Core)


#endif
