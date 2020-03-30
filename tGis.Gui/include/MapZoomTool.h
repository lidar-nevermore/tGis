#pragma once

#ifndef __MAPZOOMTOOL_H__
#define __MAPZOOMTOOL_H__

#include "tVisualize.h"
#include "tGisGuiCfg.h"

#include "QMapWidget.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class TGIS_GUI_API MapZoomTool : public MapTool
{
public:
	MapZoomTool();
	~MapZoomTool();

private:
	void SetMapWidget(IMapWidget* mapWidget);
	void MouseWheel(QMapWidget*, QWheelEvent*);
};

END_NAME_SPACE(tGis, Gui)

#endif


