#pragma once

#ifndef __PLUGIN_H__
#define __PLUGIN_H__

#include "Helper.h"

#include <string>

#if defined(_MSC_VER) || defined(__MINGW32__)
#include <Windows.h>
#else
#error Only supported under Windows currently.
#endif

using namespace std;

namespace tinyxml2
{
	class XMLDocument;
}

BEGIN_NAME_SPACE(tGis, Core)

typedef void(__cdecl *PLUGIN_INITIALIZE_PROC)(tinyxml2::XMLDocument* doc);

typedef void(__cdecl *PLUGIN_FINALIZE_PROC)();

class Plugin
{
public:
	Plugin(const char* cfg);
	~Plugin();

private:
	string _file;
	HMODULE _hModule;
	PLUGIN_INITIALIZE_PROC _initializeProc;
	PLUGIN_FINALIZE_PROC _finalizeProc;
};


END_NAME_SPACE(tGis, Core)


#endif

