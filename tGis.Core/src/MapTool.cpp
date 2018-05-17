#include "MapTool.h"

BEGIN_NAME_SPACE(tGis, Core)


MapTool::MapTool()
{
	_mapWidget = nullptr;
}


MapTool::~MapTool()
{
}

void MapTool::SetMapWidget(IMapWidget * mapWidget)
{
	_mapWidget = mapWidget;
}

bool MapTool::IsCompatible(IMapTool *)
{
	return true;
}

void MapTool::CleanUp()
{
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
