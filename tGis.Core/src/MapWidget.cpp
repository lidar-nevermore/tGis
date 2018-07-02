#include "MapWidget.h"
#include "IMap.h"
#include "IGeoSurface.h"
#include "IMapTool.h"

BEGIN_NAME_SPACE(tGis, Core)

MapWidget::MapWidget()
{
	_gridVisible = false;
	_backgroundR = 255;
	_backgroundG = 255;
	_backgroundB = 255;
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

void MapWidget::SetBackgroundColor(unsigned char R, unsigned char G, unsigned char B)
{
	_backgroundR = R;
	_backgroundG = G;
	_backgroundB = B;
}

void MapWidget::GetBackgroundColor(unsigned char * R, unsigned char * G, unsigned char * B)
{
	*R = _backgroundR;
	*G = _backgroundG;
	*B = _backgroundB;
}

void MapWidget::SetGridVisible(bool visible)
{
	_gridVisible = visible;
}

bool MapWidget::GetGridVisible()
{
	return _gridVisible;
}

void MapWidget::RepaintMap()
{
	IMap* map = this->GetMap();

	if (map == nullptr)
		return;

	IGeoSurface* surface = this->GetGeoSurface();
	map->Paint(surface);
	_overlayLayer.Paint(surface);
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


