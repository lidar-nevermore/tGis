#pragma once

#ifndef __TOOLKITREPOSITORY_H__
#define __TOOLKITREPOSITORY_H__

#include "Helper.h"

BEGIN_NAME_SPACE(tGis, Core)

class ToolKit;
class ToolKitRepositoryImpl;

class TGIS_CORE_API ToolKitRepository
{
private:
	static ToolKitRepository* _instance;
public:
	static ToolKitRepository* INSTANCE();

private:
	ToolKitRepository();
	ToolKitRepository(const ToolKitRepository &) = delete;
	ToolKitRepository &operator=(const ToolKitRepository &) = delete;

public:
	~ToolKitRepository();

public:
	void AddToolKit(int count, ...);
	void AddToolKit(ToolKit* kit);	
	size_t GetToolKitCount();
	ToolKit* GetToolKit(size_t);
	ToolKit* GetToolKit(const char* name);
	
private:
	ToolKitRepositoryImpl* _impl_;
};

END_NAME_SPACE(tGis, Core)


#endif


