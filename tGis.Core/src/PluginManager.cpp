#include "PluginManager.h"
#include "Plugin.h"

BEGIN_NAME_SPACE(tGis, Core)

PluginManager* PluginManager::_instance = nullptr;

PluginManager & PluginManager::INSTANCE()
{
	if (_instance == nullptr)
	{
		_instance = new PluginManager();
		static _tGisObjectDestructor<PluginManager> shit(_instance);
	}

	return *_instance;
}

PluginManager::PluginManager()
{
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

void PluginManager::LoadPlugins()
{
	char exePathBuffer[TGIS_MAX_PATH];
	_tgis_getcwd(exePathBuffer, TGIS_MAX_PATH);

	string pluginsPath(exePathBuffer);
	pluginsPath.append(TGIS_PATH_SEPARATOR_STR);
	pluginsPath.append("Plugin");
	pluginsPath.append(TGIS_PATH_SEPARATOR_STR);

	string findPath(pluginsPath);
	findPath.append("*.tgis.plugin");

	_tgis_finddata_t file;
	intptr_t flag;
	intptr_t handle;
	flag = handle = _tgis_findfirst(findPath.c_str(), &file);
	while (flag != -1) 
	{
		if (strcmp(file.name, ".") != 0
			&& strcmp(file.name, "..") != 0
			&& !(file.attrib&_TGIS_A_HIDDEN)
			&& !(file.attrib&_TGIS_A_SYSTEM)
			)
		{
			if (!(file.attrib&_TGIS_A_SUBDIR))
			{
				string pluginPath = pluginsPath + file.name;
				const char* csPluginPath = pluginPath.c_str();
				Plugin* plugin = nullptr;
				try 
				{
					plugin = new Plugin(csPluginPath);
					plugin->Initialize();
					_plugins.push_back(plugin);
				}
				catch (exception)
				{
				}
				LoadPluginEvent(plugin, csPluginPath);
			}
		}
		flag = _tgis_findnext(handle, &file);
	}
	_tgis_findclose(handle);
}


END_NAME_SPACE(tGis, Core)
