#pragma once

#ifndef __I_TOOL_H__
#define __I_TOOL_H__

#include "Helper.h"
#include "tGis_CoreCfg.h"

BEGIN_NAME_SPACE(tGis, Core)

struct TGIS_CORE_API ITool
{
	virtual const char* GetName() = 0;

	virtual void Execute() = 0;

	ITool() {};
	virtual ~ITool() {};
private:
	ITool(const ITool &) = delete;
	ITool &operator=(const ITool &) = delete;
};

typedef ITool* IToolPtr;

END_NAME_SPACE(tGis, Core)


#endif

