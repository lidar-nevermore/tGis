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

IOverlayLayer * MapWidget::GetOverlayLayer()
{
	return &_overlayLayer;
}

bool MapWidget::AddMapTool(IMapTool * tool)
{
	bool canAdd = true;
	for (vector<IMapTool*>::iterator it = _vecMapTool.begin(); it != _vecMapTool.end(); ++it)
	{
		if ((*it) == tool) 
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

	if (map == nullptr)
		return;

	IGeoSurface* surface = this->GetGeoSurface();
	map->Paint(surface);
	_overlayLayer.Paint(surface);
	surface->SwithSurface();
}

void MapWidget::MouseDown(void * e)
{
	for (vector<IMapTool*>::iterator it = _vecMapTool.begin(); it != _vecMapTool.end(); ++it)
	{
		if ((*it)->GetEnabled())
			(*it)->MouseDown(e);
	}
}

void MapWidget::MouseMove(void * e)
{
	for (vector<IMapTool*>::iterator it = _vecMapTool.begin(); it != _vecMapTool.end(); ++it)
	{
		if ((*it)->GetEnabled())
			(*it)->MouseMove(e);
	}
}

void MapWidget::MouseUp(void * e)
{
	for (vector<IMapTool*>::iterator it = _vecMapTool.begin(); it != _vecMapTool.end(); ++it)
	{
		if ((*it)->GetEnabled())
			(*it)->MouseUp(e);
	}
}

void MapWidget::MouseClicked(void * e)
{
	for (vector<IMapTool*>::iterator it = _vecMapTool.begin(); it != _vecMapTool.end(); ++it)
	{
		if ((*it)->GetEnabled())
			(*it)->MouseClicked(e);
	}
}

void MapWidget::MouseDoubleClicked(void * e)
{
	for (vector<IMapTool*>::iterator it = _vecMapTool.begin(); it != _vecMapTool.end(); ++it)
	{
		if ((*it)->GetEnabled())
			(*it)->MouseDoubleClicked(e);
	}
}

void MapWidget::MouseWheel(void * e)
{
	for (vector<IMapTool*>::iterator it = _vecMapTool.begin(); it != _vecMapTool.end(); ++it)
	{
		if ((*it)->GetEnabled())
			(*it)->MouseWheel(e);
	}
}

void MapWidget::KeyDown(void * e)
{
	for (vector<IMapTool*>::iterator it = _vecMapTool.begin(); it != _vecMapTool.end(); ++it)
	{
		if ((*it)->GetEnabled())
			(*it)->KeyDown(e);
	}
}

void MapWidget::KeyUp(void * e)
{
	for (vector<IMapTool*>::iterator it = _vecMapTool.begin(); it != _vecMapTool.end(); ++it)
	{
		if ((*it)->GetEnabled())
			(*it)->KeyUp(e);
	}
}

void MapWidget::KeyPress(void * e)
{
	for (vector<IMapTool*>::iterator it = _vecMapTool.begin(); it != _vecMapTool.end(); ++it)
	{
		if ((*it)->GetEnabled())
			(*it)->KeyPress(e);
	}
}


END_NAME_SPACE(tGis, Core)


