#pragma once

#ifndef __SEGMENTATIONTOOL_H__
#define __SEGMENTATIONTOOL_H__

#include "tGisCore.h"

using namespace tGis::Core;

class SegmentationTool : public ITool
{
public:
	static SegmentationTool INSTANCE;

public:
	SegmentationTool();
	~SegmentationTool();

public:
	const char* GetName();
	void Execute();
};


#endif

