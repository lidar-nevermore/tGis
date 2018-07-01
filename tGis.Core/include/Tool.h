#pragma once

#ifndef __TOOL_H__
#define __TOOL_H__

#include "Helper.h"
#include "ITool.h"

BEGIN_NAME_SPACE(tGis, Core)

class TGIS_API Tool : public ITool
{
public:
	Tool();
	virtual ~Tool();

private:
	Tool(const Tool &) = delete;
	Tool &operator=(const Tool &) = delete;

public:
	virtual const char* GetName() = 0;
	virtual void Execute() = 0;
};

END_NAME_SPACE(tGis, Core)


#endif

