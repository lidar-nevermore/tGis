#include "MapTool.h"

BEGIN_NAME_SPACE(tGis, Core)


MapTool::MapTool()
{
	_mapWidget = nullptr;
	_enabled = true;
}


MapTool::~MapTool()
{
}

void MapTool::SetMapWidget(IMapWidget * mapWidget)
{
	_mapWidget = mapWidget;
}

void MapTool::SetEnabled(bool enabled)
{
	_enabled = enabled;
}

bool MapTool::GetEnabled()
{
	return _enabled && _mapWidget != nullptr;
}

void MapTool::MouseDown(void *)
{
}

void MapTool::MouseMove(void *)
{
}

void MapTool::MouseUp(void *)
{
}

void MapTool::MouseClicked(void *)
{
}

void MapTool::MouseDoubleClicked(void *)
{
}

void MapTool::MouseWheel(void *)
{
}

void MapTool::KeyDown(void *)
{
}

void MapTool::KeyUp(void *)
{
}

void MapTool::KeyPress(void *)
{
}

END_NAME_SPACE(tGis, Core)
