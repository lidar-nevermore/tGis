#pragma once

#include "tVisualize.h"

using namespace tGis::Core;

class MapPanTool : public MapTool
{
public:
	MapPanTool();
	~MapPanTool();

public:
	void MouseDown(void*);
	void MouseMove(void*);
	void MouseUp(void*);

	using MapTool::SetMapWidget;

	using MapTool::IsCompatible;

	using MapTool::CleanUp;

	using MapTool::MouseClicked;
	using MapTool::MouseDoubleClicked;
	using MapTool::MouseWheel;

	using MapTool::KeyDown;
	using MapTool::KeyUp;
	using MapTool::KeyPress;

private:
	int _mouseDownX;
	int _mouseDownY;

	double _mouseDownSpatialX;
	double _mouseDownSpatialY;
};

