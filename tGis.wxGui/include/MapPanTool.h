#pragma once

#ifndef __MAPPANTOOL_H__
#define __MAPPANTOOL_H__

#include "tGis_Visualize.h"
#include "tGis_wxGuiCfg.h"

#include "wxGLMapWidget.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class TGIS_GUI_API MapPanTool : public MapTool
{
public:
	MapPanTool();
	~MapPanTool();

private:
	void SetMapWidget(IMapWidget* mapWidget);

	void MouseAll(wxGLMapWidget*, wxMouseEvent*);
	void MouseDown(wxGLMapWidget*, wxMouseEvent*);
	void MouseMove(wxGLMapWidget*, wxMouseEvent*);
	void MouseUp(wxGLMapWidget*, wxMouseEvent*);

private:
	int _mouseDownX;
	int _mouseDownY;

	double _mouseDownSpatialX;
	double _mouseDownSpatialY;
};


END_NAME_SPACE(tGis, Gui)


#endif

