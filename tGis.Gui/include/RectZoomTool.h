#pragma once

#ifndef __RECTZOOMTOOL_H__
#define __RECTZOOMTOOL_H__

#include "tVisualize.h"
#include "tGisGuiCfg.h"

#include "DrawRectTool.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class TGIS_GUI_API RectZoomTool : public DrawRectTool
{
public:
	RectZoomTool();
	~RectZoomTool();

public:
	virtual void MouseUp(void*);
};


END_NAME_SPACE(tGis, Gui)


#endif

