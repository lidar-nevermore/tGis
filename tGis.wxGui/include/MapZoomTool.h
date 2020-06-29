#pragma once

#ifndef __MAPZOOMTOOL_H__
#define __MAPZOOMTOOL_H__

#include "tGis_Visualize.h"
#include "tGis_wxGuiCfg.h"

#include "wxGLMapWidget.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class TGIS_GUI_API MapZoomTool : public MapTool
{
public:
	MapZoomTool();
	~MapZoomTool();

private:
	bool _delayRepaint;

private:
	void SetMapWidget(IMapWidget* mapWidget);
	void MouseWheel(wxGLMapWidget*, wxMouseEvent*);
	void MouseRightUp(wxGLMapWidget*, wxMouseEvent*);
};

END_NAME_SPACE(tGis, Gui)

#endif


