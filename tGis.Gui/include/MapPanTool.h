#pragma once

#ifndef __MAPPANTOOL_H__
#define __MAPPANTOOL_H__

#include "tVisualize.h"
#include "tGisGuiCfg.h"

#include "QMapWidget.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Gui)

class TGIS_GUI_API MapPanTool : public MapTool
{
public:
	MapPanTool();
	~MapPanTool();

private:
	void SetMapWidget(IMapWidget* mapWidget);
	void MouseDown(QMapWidget*, QMouseEvent*);
	void MouseMove(QMapWidget*, QMouseEvent*);
	void MouseUp(QMapWidget*, QMouseEvent*);

private:
	int _mouseDownX;
	int _mouseDownY;

	double _mouseDownSpatialX;
	double _mouseDownSpatialY;
};


END_NAME_SPACE(tGis, Gui)


#endif

