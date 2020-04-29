#pragma once

#ifndef __TOOLKITREPOSITORY_H__
#define __TOOLKITREPOSITORY_H__

#include "Helper.h"
#include "ToolKitSet.h"

BEGIN_NAME_SPACE(tGis, Core)

class ToolKitRepositoryImpl;

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
	void AddToolKit(int count, ...);
	void SaveStandaloneTool();
	using ToolKitSet::AddToolKit;
	
private:
	ToolKitRepositoryImpl* _impl_;
};

END_NAME_SPACE(tGis, Core)


#endif


