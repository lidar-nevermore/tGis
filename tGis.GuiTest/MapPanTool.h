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

private:
	int _mouseDownX;
	int _mouseDownY;

	double _mouseDownSpatialX;
	double _mouseDownSpatialY;
};

