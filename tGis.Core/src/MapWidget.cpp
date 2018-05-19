#include "MapWidget.h"
#include "IMap.h"
#include "IGeoSurface.h"
#include "IMapTool.h"


BEGIN_NAME_SPACE(tGis, Core)

MapWidget::MapWidget()
{
}


MapWidget::~MapWidget()
{
}

bool MapWidget::AddMapTool(IMapTool * tool)
{
	bool canAdd = true;
	for (vector<IMapTool*>::iterator it = _vecMapTool.begin(); it != _vecMapTool.end(); ++it)
	{
		if (!(*it)->IsCompatible(tool)) 
		{
			canAdd = false;
			break;
		}
	}

	if (canAdd)
	{
		tool->SetMapWidget(this);
		_vecMapTool.push_back(tool);
	}

	return canAdd;
}

void MapWidget::RemoveMapTool(IMapTool * tool)
{
	for (vector<IMapTool*>::iterator it = _vecMapTool.begin(); it != _vecMapTool.end(); ++it)
	{
		if (*it == tool)
		{
			(*it)->SetMapWidget(nullptr);
			_vecMapTool.erase(it);
			break;
		}
	}
}

void MapWidget::RepaintMap()
{
	IMap* map = this->GetMap();
	IGeoSurface* surface = this->GetGeoSurface();
	map->Paint(surface);
	surface->SwithSurface();
}

void MapWidget::MouseDown(void * e)
{
	for (vector<IMapTool*>::iterator it = _vecMapTool.begin(); it != _vecMapTool.end(); ++it)
	{
		(*it)->MouseDown(e);
	}
}

void MapWidget::MouseMove(void * e)
{
	for (vector<IMapTool*>::iterator it = _vecMapTool.begin(); it != _vecMapTool.end(); ++it)
	{
		(*it)->MouseMove(e);
	}
}

void MapWidget::MouseUp(void * e)
{
	for (vector<IMapTool*>::iterator it = _vecMapTool.begin(); it != _vecMapTool.end(); ++it)
	{
		(*it)->MouseUp(e);
	}
}

void MapWidget::MouseClicked(void * e)
{
	for (vector<IMapTool*>::iterator it = _vecMapTool.begin(); it != _vecMapTool.end(); ++it)
	{
		(*it)->MouseClicked(e);
	}
}

void MapWidget::MouseDoubleClicked(void * e)
{
	for (vector<IMapTool*>::iterator it = _vecMapTool.begin(); it != _vecMapTool.end(); ++it)
	{
		(*it)->MouseDoubleClicked(e);
	}
}

void MapWidget::MouseWheel(void * e)
{
	for (vector<IMapTool*>::iterator it = _vecMapTool.begin(); it != _vecMapTool.end(); ++it)
	{
		(*it)->MouseWheel(e);
	}
}

void MapWidget::KeyDown(void * e)
{
	for (vector<IMapTool*>::iterator it = _vecMapTool.begin(); it != _vecMapTool.end(); ++it)
	{
		(*it)->KeyDown(e);
	}
}

void MapWidget::KeyUp(void * e)
{
	for (vector<IMapTool*>::iterator it = _vecMapTool.begin(); it != _vecMapTool.end(); ++it)
	{
		(*it)->KeyUp(e);
	}
}

void MapWidget::KeyPress(void * e)
{
	for (vector<IMapTool*>::iterator it = _vecMapTool.begin(); it != _vecMapTool.end(); ++it)
	{
		(*it)->KeyPress(e);
	}
}


END_NAME_SPACE(tGis, Core)


