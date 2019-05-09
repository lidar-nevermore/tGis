#pragma once

#ifndef __REGIONMERGINGTOOL_H__
#define __REGIONMERGINGTOOL_H__

#include "tGisCore.h"

using namespace tGis::Core;

class RegionMergingTool : public ITool
{
public:
	static RegionMergingTool INSTANCE;

public:
	RegionMergingTool();
	~RegionMergingTool();

public:
	const char* GetName();
	void Execute();
};


#endif

