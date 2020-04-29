#pragma once

#ifndef __ToolKitSet_H__
#define __ToolKitSet_H__

#include "Helper.h"
#include "IInclusionObject.h"

BEGIN_NAME_SPACE(tGis, Core)

class ToolKit;
class ToolKitSetImpl;

class TGIS_CORE_API ToolKitSet
{
	friend class ToolKitRepository;
	TGIS_DECLARE_NO_COPY_CLASS(ToolKitSet);
	
protected: //ToolKitSet不让实例化，只能继承
	ToolKitSet();

public:
	virtual ~ToolKitSet();

public:
	virtual void AddToolKit(ToolKit* kit);
	virtual size_t GetToolKitCount();
	virtual ToolKit* GetToolKit(size_t);
	virtual ToolKit* GetToolKit(const char* name);
	virtual void RemoveToolKit(ToolKit* kit);

protected:
	ToolKitSetImpl* _impl_;
};

END_NAME_SPACE(tGis, Core)


#endif


