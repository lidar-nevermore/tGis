#pragma once

#include "tVisualize.h"

using namespace tGis::Core;

class MapZoomTool : public MapTool
{
public:
	MapZoomTool();
	~MapZoomTool();

	void MouseWheel(void*);
};

