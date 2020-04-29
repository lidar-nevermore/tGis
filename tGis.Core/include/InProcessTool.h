#pragma once

#ifndef __InProcessTool_H__
#define __InProcessTool_H__

#include "Helper.h"
#include "ITool.h"

BEGIN_NAME_SPACE(tGis, Core)

//动态链接库插件中提供的工具需要继承自InProcessTool
class TGIS_CORE_API InProcessTool : public ITool
{
	TGIS_DECLARE_NO_COPY_CLASS(InProcessTool);

public:
	virtual const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);

private:
	static const char* const _type;

public:
	InProcessTool();
	virtual ~InProcessTool();

public:
	virtual const char* GetName() = 0;
	virtual void Execute() = 0;
};

END_NAME_SPACE(tGis, Core)


#endif

