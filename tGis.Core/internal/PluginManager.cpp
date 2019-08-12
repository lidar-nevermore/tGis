#include "PluginManager.h"
#include "Plugin.h"
#include <memory>

BEGIN_NAME_SPACE(tGis, Core)

PluginManager* PluginManager::_instance = nullptr;

PluginManager & PluginManager::INSTANCE()
{
	if (_instance == nullptr)
	{
		_instance = new PluginManager();
		static unique_ptr<PluginManager> shit(_instance);
	}

	return *_instance;
}

PluginManager::PluginManager()
{
	char exePathBuffer[TGIS_MAX_PATH];
	_tgis_getcwd(exePathBuffer, TGIS_MAX_PATH);

	_pluginsPath = exePathBuffer;
	_pluginsPath.append(TGIS_PATH_SEPARATOR_STR);
	_pluginsPath.append("Plugins");
	_pluginsPath.append(TGIS_PATH_SEPARATOR_STR);
}


PluginManager::~PluginManager()
{
	for (vector<Plugin*>::iterator it = _plugins.begin(); it != _plugins.end(); it++)
	{
		Plugin* plugin = *it;

		plugin->Finalize();
		delete plugin;
	}
}

void PluginManager::OnTraverseDir(void * usr, const char * dir, const char * name, unsigned int attrib)
{
	PluginManager* pm = (PluginManager*)usr;

	if (!(attrib&_TGIS_A_HIDDEN)
		&& !(attrib&_TGIS_A_SYSTEM)
		&& !(attrib&_TGIS_A_SUBDIR))
	{
		string pluginPath = pm->_pluginsPath + name;
		const char* csPluginPath = pluginPath.c_str();
		Plugin* plugin = nullptr;
		plugin = new Plugin(csPluginPath);
		plugin->Initialize();
		pm->_plugins.push_back(plugin);
	}
}

void PluginManager::LoadPlugins()
{
	_tgis_traverse_dir(_pluginsPath.c_str(), "*.tgis.plugin", this, OnTraverseDir);
}


END_NAME_SPACE(tGis, Core)
