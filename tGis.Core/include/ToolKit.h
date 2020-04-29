#pragma once

#ifndef __TOOLKIT_H__
#define __TOOLKIT_H__

#include "Helper.h"
#include "IInclusionObject.h"
#include "ToolKitSet.h"

BEGIN_NAME_SPACE(tGis, Core)

struct ITool;
class ToolKitImpl;
class StandaloneTool;

class TGIS_CORE_API ToolKit : public ToolKitSet, public IInclusionObject
{
	TGIS_DECLARE_NO_COPY_CLASS(ToolKit);

	friend class ToolKitSet;

public:
	ToolKit(const char* name);
	~ToolKit();

public:
	const char* GetName();
	ToolKit* GetParent();
	void AddTool(ITool* tool);
	size_t GetToolCount();
	ITool* GetTool(size_t);
	void RemoveTool(ITool* tool);

public:
	void AddToolKit(ToolKit* kit) override;

private:
	ToolKit* _parent;
	ToolKitImpl* _impl_;
};

typedef ToolKit* ToolKitPtr;

END_NAME_SPACE(tGis, Core)


#endif


