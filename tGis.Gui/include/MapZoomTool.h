#pragma once

#ifndef __MAPZOOMTOOL_H__
#define __MAPZOOMTOOL_H__

#include "tVisualize.h"
#include "tGisGuiCfg.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class TGIS_GUI_API MapZoomTool : public MapTool
{
public:
	MapZoomTool();
	~MapZoomTool();

	void MouseWheel(void*);
};

END_NAME_SPACE(tGis, Gui)

#endif


