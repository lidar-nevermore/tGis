#pragma once

#ifndef __ToolKitSet_H__
#define __ToolKitSet_H__

#include "Helper.h"
#include "IInclusionObject.h"

BEGIN_NAME_SPACE(tGis, Core)

struct ITool;
class ToolKit;
class ToolKitSetImpl;

class TGIS_CORE_API ToolKitSet
{
	TGIS_DECLARE_NO_COPY_CLASS(ToolKitSet);

	friend class ToolKit;
public:
	ToolKitSet();
	virtual ~ToolKitSet();

public:
	virtual ToolKitSet* GetParent();

	virtual void AddToolKit(ToolKit* kit);
	virtual size_t GetToolKitCount();
	virtual ToolKit* GetToolKit(size_t);
	virtual ToolKit* GetToolKit(const char* name);
	virtual void RemoveToolKit(ToolKit* kit);

protected:
	ToolKitSet* _parent;

private:
	ToolKitSetImpl* _impl_;
};

END_NAME_SPACE(tGis, Core)


#endif


