#include "TGisApplication.h"
#include "PluginManager.h"
#include "ilog.h"
#include <stdarg.h>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

TGisApplication* TGisApplication::_instance = nullptr;

Event<const char*, const char*, const char*, const char*> TGisApplication::LoadPluginEvent;

TGisApplication & TGisApplication::INSTANCE()
{
	return *_instance;
}

TGisApplication::TGisApplication()
{
	if (_instance != NULL)
		throw exception("Only allow one instance!");
	_tgis_getcwd(_exeDir, TGIS_MAX_PATH);
	std::string logPath = _exeDir;
	logPath.append(TGIS_PATH_SEPARATOR_STR);
	logPath.append("Log");
	logPath.append(TGIS_PATH_SEPARATOR_STR);
	logPath.append("tgis.");
	_loger = init_log(logPath.c_str(), LOG_DEBUG, 0);
	_instance = this;
	PluginManager::INSTANCE().LoadPlugins();
}

TGisApplication::~TGisApplication()
{
	finalize_log(_loger);
}


END_NAME_SPACE(tGis, Core)
