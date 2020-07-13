#include "Plugin.h"
#include "PluginManager.h"
#include "tGisApplication.h"

#include <io.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <locale>         // std::wstring_convert
#include <codecvt>        // std::codecvt_utf8


#include "tinyxml2.h"

using namespace tinyxml2;

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

Plugin::Plugin(const char* cfg)
{
	bool cfgOk = false;
	tinyxml2::XMLDocument doc;
	if (XML_SUCCESS == doc.LoadFile(cfg))
	{
		XMLElement* eleBin = doc.FirstChildElement("bin");
		if (eleBin != nullptr)
		{
			_file = eleBin->GetText();
			cfgOk = true;
		}
	}

	if (cfgOk == false)
	{
		TGIS_LOG_FORMAT(LOG_INFO, "loading...\ncfg=%s\nConfiguration is invalid!", cfg);
		return;
	}

	if (_file.compare(0, 7, "$(tGis)") == 0)
	{
		string relpath = _file.substr(7);

		_file = tGisApplication::INSTANCE()->GetExeDir();
		//_file.append(TGIS_PATH_SEPARATOR_STR);
		_file.append(relpath);
	}

	try
	{
		_hModule = LoadLibraryExA(_file.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
		if (_hModule != NULL)
		{
			_initializeProc = reinterpret_cast<PLUGIN_INITIALIZE_PROC>(::GetProcAddress(_hModule, "_tGisPluginInitialize"));
			_finalizeProc = reinterpret_cast<PLUGIN_FINALIZE_PROC>(::GetProcAddress(_hModule, "_tGisPluginFinalize"));
		}

		const char* result = "OK";
		if (_hModule == NULL)
			result = "Loading module failed!";
		else if (_initializeProc == NULL || _finalizeProc == NULL)
			result = "Not a valid plugin!";
		else
			_initializeProc(&doc);

		TGIS_LOG_FORMAT(LOG_INFO, "loading...\ncfg=%s\nbin=%s\nresult=%s", cfg, _file.c_str(), result);
		tGisApplication::LoadPluginEvent(cfg, _file.c_str(), result);
	}
	catch (exception &ex)
	{
		string msg = "exception:";
		msg.append(ex.what());
		const char* result = msg.c_str();
		TGIS_LOG_FORMAT(LOG_INFO, "loading...\ncfg=%s\nbin=%s\nresult=%s", cfg, _file.c_str(), result);
		tGisApplication::LoadPluginEvent(cfg, _file.c_str(), result);
	}
	catch (...)
	{
		const char* result = "unknown error!";
		TGIS_LOG_FORMAT(LOG_INFO, "loading...\ncfg=%s\nbin=%s\nresult=%s", cfg, _file.c_str(), result);
		tGisApplication::LoadPluginEvent(cfg, _file.c_str(), result);
	}
}


Plugin::~Plugin()
{
	if (_initializeProc != NULL && _finalizeProc != NULL)
		_finalizeProc();

	if (_hModule != NULL)
		FreeLibrary(_hModule);
}


END_NAME_SPACE(tGis, Core)

