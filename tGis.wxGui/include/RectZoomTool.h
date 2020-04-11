#pragma once

#ifndef __RECTZOOMTOOL_H__
#define __RECTZOOMTOOL_H__

#include "tGis_Visualize.h"
#include "tGis_wxGuiCfg.h"

#include "DrawRectTool.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class TGIS_GUI_API RectZoomTool : public DrawRectTool
{
public:
	RectZoomTool();
	~RectZoomTool();

private:
	void MouseUp(wxGLMapWidget* s, wxMouseEvent *);
};


END_NAME_SPACE(tGis, Gui)


#endif

