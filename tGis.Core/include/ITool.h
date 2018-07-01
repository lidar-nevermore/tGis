#pragma once

#ifndef __I_TOOL_H__
#define __I_TOOL_H__

#include "Helper.h"

BEGIN_NAME_SPACE(tGis, Core)

struct TGIS_API ITool
{
	virtual const char* GetName() = 0;

	virtual void Execute() = 0;

	ITool() {};
	virtual ~ITool() {};
private:
	ITool(const ITool &) = delete;
	ITool &operator=(const ITool &) = delete;
};

END_NAME_SPACE(tGis, Core)


#endif

