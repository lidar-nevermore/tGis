#pragma once

#ifndef __RASTERSUBSETTOOL_H__
#define __RASTERSUBSETTOOL_H__

#include "tVisualize.h"
#include "tGisGuiCfg.h"

#include "DrawRectTool.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class TGIS_GUI_API RasterSubsetTool : public DrawRectTool
{
public:
	RasterSubsetTool();
	~RasterSubsetTool();
};


END_NAME_SPACE(tGis, Gui)


#endif


