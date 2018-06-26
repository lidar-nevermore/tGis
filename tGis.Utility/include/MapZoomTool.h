#pragma once

#ifndef __MAPZOOMTOOL_H__
#define __MAPZOOMTOOL_H__

#include "tVisualize.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Utility)

class TGIS_UTILITY_API MapZoomTool : public MapTool
{
public:
	MapZoomTool();
	~MapZoomTool();

	void MouseWheel(void*);
};

END_NAME_SPACE(tGis, Utility)

#endif


