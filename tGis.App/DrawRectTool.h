#pragma once

#include "tVisualize.h"

using namespace tGis::Core;

class DrawRectTool : public MapTool
{
public:
	DrawRectTool();
	virtual ~DrawRectTool();

public:
	virtual void SetMapWidget(IMapWidget* mapWidget);

	virtual void MouseDown(void*);
	virtual void MouseMove(void*);
	virtual void MouseUp(void*);


protected:
	OverlayRect _rect;
};

