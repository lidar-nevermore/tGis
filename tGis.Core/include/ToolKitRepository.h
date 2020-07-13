#pragma once

#ifndef __TOOLKITREPOSITORY_H__
#define __TOOLKITREPOSITORY_H__

#include "Helper.h"
#include "ToolKitSet.h"

namespace tinyxml2
{
	class XMLDocument;
}

BEGIN_NAME_SPACE(tGis, Core)

struct ITool;

class TGIS_CORE_API ToolKitRepository : public ToolKitSet
{
	TGIS_DECLARE_NO_COPY_CLASS(ToolKitRepository);

private:
	static ToolKitRepository* _instance;
public:
	static ToolKitRepository* INSTANCE();

private:
	ToolKitRepository();

public:
	~ToolKitRepository();

public:
	void AddTool(const char* belong, ITool* tool);
	void AddToolKit(int count, ...);
	void SaveStandaloneTool();
	using ToolKitSet::AddToolKit;

private:
	void SaveStandaloneTool(tinyxml2::XMLDocument* doc, ToolKitSet* kitSet);

};

END_NAME_SPACE(tGis, Core)


#endif


