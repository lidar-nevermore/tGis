#pragma once

#ifndef __WATEREXTRACTIONTOOL_H__
#define __WATEREXTRACTIONTOOL_H__

#include "tGisCore.h"

using namespace tGis::Core;

class WaterExtractionTool : public ITool
{
public:
	static WaterExtractionTool INSTANCE;

public:
	WaterExtractionTool();
	~WaterExtractionTool();

public:
	const char* GetName();
	void Execute();
};


#endif

