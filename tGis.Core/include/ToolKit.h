#pragma once

#ifndef __TOOLKIT_H__
#define __TOOLKIT_H__

#include "Helper.h"
#include "tGisCoreCfg.h"

#include <vector>
#include <map>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

struct ITool;

class TGIS_CORE_API ToolKit
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
	size_t GetToolCount();
	ITool* GetTool(size_t);

	void AddToolKit(ToolKit* kit);
	size_t GetToolKitCount();
	ToolKit* GetToolKit(size_t);
	ToolKit* GetToolKit(const char* name);

private:
	string _name;
	vector<ToolKit*> _vecToolKit;
	map<string, ToolKit*> _mapToolKit;
	vector<ITool*> _vecTool;
};

typedef ToolKit* ToolKitPtr;

END_NAME_SPACE(tGis, Core)


#endif


