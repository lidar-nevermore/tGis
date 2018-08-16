#include "Plugin.h"

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
					_tgis_str_split(line, "=", fileds);

					if (fileds.size() == 2) 
					{
						if (fileds[0].compare("name") == 0)
							_name = fileds[1];
						else if (fileds[0].compare("path") == 0)
							_path = fileds[1];
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
	}
	catch (exception &ex)
	{
		file.close();
		throw ex;
	}
	file.close();
	std::wstring_convert<std::codecvt_utf8<wchar_t>> wstr_converter;
	wstring wpath = wstr_converter.from_bytes(_path);
	_hModule = LoadLibraryEx((LPCWSTR)(wpath.c_str()), NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
	if (_hModule == NULL)
		throw exception("º”‘ÿ≤Âº˛ ß∞‹£°");
	_initializeProc = reinterpret_cast<PLUGIN_INITIALIZE_PROC>(::GetProcAddress(_hModule, "PluginInitialize"));
	_finalizeProc = reinterpret_cast<PLUGIN_FINALIZE_PROC>(::GetProcAddress(_hModule, "PluginFinalize"));
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
	_initializeProc();
}

void Plugin::Finalize()
{
	_finalizeProc();
}


END_NAME_SPACE(tGis, Core)

