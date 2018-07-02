#include "MapTool.h"
#include "IMapWidget.h"

BEGIN_NAME_SPACE(tGis, Core)


MapTool::MapTool()
{
	_mapWidget = nullptr;
	_enabled = true;
}


MapTool::~MapTool()
{
}

IMapWidget * MapTool::GetMapWidget()
{
	return _mapWidget;
}

void MapTool::SetMapWidget(IMapWidget * mapWidget)
{
	_mapWidget = mapWidget;
}

void MapTool::SetEnabled(bool enabled)
{
	bool raise = _enabled != enabled;
	_enabled = enabled;
	if (raise && _mapWidget != nullptr)
	{
		IMapTool* mt = this;
		_mapWidget->MapToolChangedEvent(_mapWidget, mt);
	}
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


