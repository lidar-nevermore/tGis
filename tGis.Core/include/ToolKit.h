#pragma once

#ifndef __TOOLKIT_H__
#define __TOOLKIT_H__

#include "Helper.h"

BEGIN_NAME_SPACE(tGis, Core)

struct ITool;
class ToolKitImpl;

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
	ToolKitImpl* _impl_;
};

typedef ToolKit* ToolKitPtr;

END_NAME_SPACE(tGis, Core)


#endif


