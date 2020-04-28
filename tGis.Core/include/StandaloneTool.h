#pragma once

#ifndef __TOOL_H__
#define __TOOL_H__

#include "Helper.h"
#include "ITool.h"

BEGIN_NAME_SPACE(tGis, Core)

class StandaloneToolImpl;

class TGIS_CORE_API StandaloneTool : public ITool
{
public:
	StandaloneTool(const char* name, ToolKit* parent);
	virtual ~StandaloneTool();

private:
	StandaloneTool(const StandaloneTool &) = delete;
	StandaloneTool &operator=(const StandaloneTool &) = delete;

public:
	virtual const char* GetName() = 0;
	virtual void Execute() = 0;

public:
	virtual void SetExeFile(const char* exe);
	virtual const char* GetExeFile();
	virtual size_t GetParamCount();
	virtual const char* GetParam(size_t pos);
	virtual void AddParam(const char* param);
	virtual void RemoveParam(size_t pos);
	virtual void ClearParam();

private:
	StandaloneToolImpl* _impl_;
};

END_NAME_SPACE(tGis, Core)


#endif

