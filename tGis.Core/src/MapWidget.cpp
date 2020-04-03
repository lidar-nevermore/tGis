#include "MapWidget.h"
#include "IMap.h"
#include "ILayer.h"
#include "IDataset.h"
#include "GeoSurface.h"
#include "IMapTool.h"
#include "MapTool.h"


BEGIN_NAME_SPACE(tGis, Core)

MapWidget::MapWidget()
{
	_gridVisible = false;
	_backgroundR = 255;
	_backgroundG = 255;
	_backgroundB = 255;
	_map = nullptr;
	_geoSurface = nullptr;
}


MapWidget::~MapWidget()
{
	vector<IMapTool*>::iterator it = _vecMapTool.begin(); 
	while(it != _vecMapTool.end())
	{
		IMapTool* tool = *it;
		tool->SetMapWidget(nullptr);
		it = _vecMapTool.erase(it);
		MapToolRemovedEvent(this, tool);
	}
}

void MapWidget::SetMap(IMap *map)
{
	if (map == nullptr && _map != nullptr)
	{
		map->LayerAddedEvent.Remove(this, &MapWidget::LayerAdded);
		map->LayerClearedEvent.Remove(this, &MapWidget::LayerCleared);
		map->LayerRemovedEvent.Remove(this, &MapWidget::LayerRemoved);
	}
	_map = map;
	if (_map != nullptr)
	{
		map->LayerAddedEvent.Add(this, &MapWidget::LayerAdded);
		map->LayerClearedEvent.Add(this, &MapWidget::LayerCleared);
		map->LayerRemovedEvent.Add(this, &MapWidget::LayerRemoved);
	}	
}

void MapWidget::LayerAdded(IMapPtr map, ILayerPtr layer)
{
	if (map->GetLayerCount() == 1)
	{
		_viewPort.SetSpatialReference(layer->GetDataset()->GetSpatialReference());
	}
	const OGREnvelope* envelope = layer->GetEnvelope();
	_viewPort.IncludeEnvelope(envelope);
	RepaintMap();
}

void MapWidget::LayerRemoved(IMapPtr map, ILayerPtr layer)
{
	if (map->GetLayerCount() == 0)
	{
		_viewPort.SetSpatialReference(nullptr);
	}
	RepaintMap();
}

void MapWidget::LayerCleared(IMapPtr)
{
	_viewPort.SetSpatialReference(nullptr);
	RepaintMap();
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
		MapToolAddedEvent(this, std::forward<IMapTool*>(tool));
	}

	return canAdd;
}

bool MapWidget::RemoveMapTool(IMapTool * tool)
{
	for (vector<IMapTool*>::iterator it = _vecMapTool.begin(); it != _vecMapTool.end(); ++it)
	{
		if (*it == tool)
		{
			tool->SetMapWidget(nullptr);
			_vecMapTool.erase(it);
			MapToolRemovedEvent(this, std::forward<IMapTool*>(tool));
			return true;
		}
	}

	return false;
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

	_geoSurface->SetViewPort(&_viewPort);
	_geoSurface->BeginPaint(this,false);
	map->Paint(_geoSurface);
	_geoSurface->EndPaint(this,false);
}

void MapWidget::PresentMap()
{
	double surfSpatialLeft;
	double surfSpatialTop;
	double surfSpatialRight;
	double surfSpatialBottom;

	_geoSurface->GetViewPort()->GetEnvelope(&surfSpatialLeft, &surfSpatialTop, &surfSpatialRight, &surfSpatialBottom);
	
	double surfPresentLeft;
	double surfPresentTop;
	double surfPresentRight;
	double surfPresentBottom;
	
	_viewPort.Spatial2Surface(surfSpatialLeft, surfSpatialTop, &surfPresentLeft, &surfPresentTop);
	_viewPort.Spatial2Surface(surfSpatialRight, surfSpatialBottom, &surfPresentRight, &surfPresentBottom);

	int surfPresentWidth = (int)_tgis_round(surfPresentRight - surfPresentLeft, 0);
	int surfPresentHeight = (int)_tgis_round(surfPresentBottom - surfPresentTop, 0);

	_geoSurface->BeginPaint(this,true);
	_geoSurface->Present(this, (int)surfPresentLeft, (int)surfPresentTop, surfPresentWidth, surfPresentHeight);
	_geoSurface->EndPaint(this,true);
}



END_NAME_SPACE(tGis, Core)


