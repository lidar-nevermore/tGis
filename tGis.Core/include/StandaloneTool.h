#pragma once

#ifndef __TOOL_H__
#define __TOOL_H__

#include "Helper.h"
#include "ITool.h"
#include "Event.h"

BEGIN_NAME_SPACE(tGis, Core)

class StandaloneToolImpl;


class TGIS_CORE_API StandaloneTool : public ITool
{

public:
	virtual const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);

private:
	static const char* const _type;

public:
	StandaloneTool(const char* name);
	StandaloneTool(const StandaloneTool& tool);
	StandaloneTool& operator=(const StandaloneTool& tool);
	virtual ~StandaloneTool();

public:
	virtual const char* GetName();
	virtual void Execute();

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

