#pragma once

#ifndef __TOOLKIT_H__
#define __TOOLKIT_H__

#include "Helper.h"
#include <vector>
#include <map>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

struct ITool;

class TGIS_API ToolKit
{
public:
	ToolKit(const char* name);
	~ToolKit();
private:
	ToolKit(const ToolKit &) = delete;
	ToolKit &operator=(const ToolKit &) = delete;

public:
	const char* GetName();

	void AddTool(ITool* tool);
	int GetToolCount();
	ITool* GetTool(int);

	void AddToolKit(ToolKit* kit);
	int GetToolKitCount();
	ToolKit* GetToolKit(int);
	ToolKit* GetToolKit(const char* name);

private:
	string _name;
	vector<ToolKit*> _vecToolKit;
	map<string, ToolKit*> _mapToolKit;
	vector<ITool*> _vecTool;
};

END_NAME_SPACE(tGis, Core)


#endif


