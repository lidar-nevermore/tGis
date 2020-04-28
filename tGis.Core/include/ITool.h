#pragma once

#ifndef __I_TOOL_H__
#define __I_TOOL_H__

#include "Helper.h"
#include "ITypedObject.h"
#include "IInclusionObject.h"

BEGIN_NAME_SPACE(tGis, Core)

class ToolKit;

struct TGIS_CORE_API ITool : public IInclusionObject, public ITypedObject
{
	friend class ToolKit;

	virtual const char* GetName() = 0;

	virtual void Execute() = 0;

	virtual ToolKit* GetParent()
	{
		return _parent;
	};

	ITool(ToolKit* parent) 
	{
		_parent = parent;
	};
	virtual ~ITool() {};

protected:
	ToolKit* _parent;

private:
	ITool(const ITool &) = delete;
	ITool &operator=(const ITool &) = delete;
};

typedef ITool* IToolPtr;

END_NAME_SPACE(tGis, Core)


#endif

