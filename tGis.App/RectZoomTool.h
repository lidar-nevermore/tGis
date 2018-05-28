#pragma once

#include "tVisualize.h"

#include "DrawRectTool.h"

using namespace tGis::Core;

class RectZoomTool : public DrawRectTool
{
public:
	RectZoomTool();
	~RectZoomTool();

public:
	virtual void MouseUp(void*);
};

