#include "Plugin.h"
#include "PluginManager.h"
#include "TGisApplication.h"

#include <io.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <locale>         // std::wstring_convert
#include <codecvt>        // std::codecvt_utf8
using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

Plugin::Plugin(const char* cfg)
{
	const size_t len = 32768; // 512k
	char buffer[len] = { 0 };
	ifstream file;
	file.open(cfg, ios::binary);

	try
	{
		size_t readSize = 0;
		size_t remainSize = 0;
		do
		{
			file.read(buffer + remainSize, len);
			readSize = file.gcount();
			char* line = buffer;
			size_t totalSize = readSize + remainSize;
			remainSize = totalSize;
			for (size_t i = 0; i < totalSize; i++)
			{
				if (buffer[i] == '\n')
				{
					buffer[i] = '\0';
					vector<string> fileds;
					_tgis_str_split(line, "=\r\n", fileds);

					if (fileds.size() == 2) 
					{
						if (fileds[0].compare("name") == 0)
							_name = fileds[1];
						else if (fileds[0].compare("file") == 0)
							_file = PluginManager::INSTANCE().GetPluginsPath() + fileds[1];
					}

					line = buffer + i + 1;
					remainSize = totalSize - i - 1;
				}
			}
			if (remainSize > 0 && remainSize < totalSize)
			{
				memcpy(buffer, buffer + (totalSize - remainSize), remainSize);
			}
		} while (readSize == len);

		file.close();
		std::wstring_convert<std::codecvt_utf8<wchar_t>> wstr_converter;
		wstring wpath = wstr_converter.from_bytes(_file);
		_hModule = LoadLibraryEx((LPCWSTR)(wpath.c_str()), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
		if (_hModule != NULL)
		{
			_initializeProc = reinterpret_cast<PLUGIN_INITIALIZE_PROC>(::GetProcAddress(_hModule, "_tGisPluginInitialize"));
			_finalizeProc = reinterpret_cast<PLUGIN_FINALIZE_PROC>(::GetProcAddress(_hModule, "_tGisPluginFinalize"));
		}

		const char* plugin_name = _name.c_str();
		const char* plugin_file = _file.c_str();
		const char* result = NULL;
		if (_hModule == NULL)
			result = "Loading module failed!";

		if (_initializeProc == NULL || _finalizeProc == NULL)
			result = "Not a valid plugin!";

		TGisApplication::LoadPluginEvent(cfg, plugin_name, plugin_file, result);
	}
	catch (exception &ex)
	{
		file.close();
		string msg = "exception:";
		msg.append(ex.what());
		const char* plugin_name = NULL;
		const char* plugin_file = NULL;
		const char* result = msg.c_str();
		TGisApplication::LoadPluginEvent(cfg, plugin_name, plugin_file, result);
	}
}


Plugin::~Plugin()
{
	if (_hModule != NULL)
		FreeLibrary(_hModule);
}

const char * Plugin::GetName()
{
	return _name.c_str();
}

void Plugin::Initialize()
{
	if (_initializeProc != NULL && _finalizeProc != NULL)
		_initializeProc();
}

void Plugin::Finalize()
{
	if (_initializeProc != NULL && _finalizeProc != NULL)
		_finalizeProc();
}


END_NAME_SPACE(tGis, Core)

