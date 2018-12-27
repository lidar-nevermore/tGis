#pragma once

#ifndef __PLUGIN_H__
#define __PLUGIN_H__

#include "Helper.h"

#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

typedef void(__cdecl *PLUGIN_INITIALIZE_PROC)();

typedef void(__cdecl *PLUGIN_FINALIZE_PROC)();

class Plugin
{
public:
	Plugin(const char* cfg);
	~Plugin();

private:
	string _name; // utf8
	string _file; // utf8
	HMODULE _hModule;
	PLUGIN_INITIALIZE_PROC _initializeProc;
	PLUGIN_FINALIZE_PROC _finalizeProc;
public:
	const char* GetName();
	void Initialize();
	void Finalize();
};


END_NAME_SPACE(tGis, Core)


#endif

