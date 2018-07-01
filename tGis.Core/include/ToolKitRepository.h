#pragma once

#ifndef __TOOLKITREPOSITORY_H__
#define __TOOLKITREPOSITORY_H__

#include "Helper.h"

#include <vector>
#include <map>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

class ToolKit;

class TGIS_API ToolKitRepository
{
public:
	static ToolKitRepository* _instance;
	static ToolKitRepository& INSTANCE();

private:
	ToolKitRepository();
	ToolKitRepository(const ToolKitRepository &) = delete;
	ToolKitRepository &operator=(const ToolKitRepository &) = delete;

public:
	~ToolKitRepository();

public:
	void AddToolKit(ToolKit* kit);
	int GetToolKitCount();
	ToolKit* GetToolKit(int);
	ToolKit* GetToolKit(const char* name);

private:
	vector<ToolKit*> _vecToolKit;
	map<string, ToolKit*> _mapToolKit;
};

END_NAME_SPACE(tGis, Core)


#endif


