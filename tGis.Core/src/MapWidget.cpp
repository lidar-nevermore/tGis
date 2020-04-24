#include "MapWidget.h"
#include "IMap.h"
#include "ILayer.h"
#include "IDataset.h"
#include "GeoSurface.h"
#include "IMapTool.h"
#include "MapTool.h"


BEGIN_NAME_SPACE(tGis, Core)

class MapWidgetImpl
{
public:
	MapWidgetImpl(MapWidget* owner)
	{
		_owner = owner;
	}

	MapWidget* _owner;

	vector<IMapTool*> _vecMapTool;
};

MapWidget::MapWidget()
{
	_impl_ = new MapWidgetImpl(this);
	_gridVisible = false;
	_backgroundR = 255;
	_backgroundG = 255;
	_backgroundB = 255;
	_map = nullptr;
	_geoSurface = nullptr;
}


MapWidget::~MapWidget()
{
	vector<IMapTool*>::iterator it = _impl_->_vecMapTool.begin();
	while(it != _impl_->_vecMapTool.end())
	{
		IMapTool* tool = *it;
		tool->SetMapWidget(nullptr);
		it = _impl_->_vecMapTool.erase(it);
		MapToolRemovedEvent(this, tool);
	}
	MapWidget::SetMap(nullptr);
	delete _impl_;
}

void MapWidget::SetMap(IMap *map)
{
	if (_map == map)
		return;

	if (_map != nullptr)
	{
		_viewPort.SetSpatialReference(nullptr);
		_map->LayerAddedEvent.Remove(this, &MapWidget::LayerAdded);
		_map->LayerClearedEvent.Remove(this, &MapWidget::LayerCleared);
		_map->LayerRemovedEvent.Remove(this, &MapWidget::LayerRemoved);
	}
	_map = map;
	if (_map != nullptr)
	{
		_viewPort.SetSpatialReference(map->GetSpatialReference());
		_map->LayerAddedEvent.Add(this, &MapWidget::LayerAdded);
		_map->LayerClearedEvent.Add(this, &MapWidget::LayerCleared);
		_map->LayerRemovedEvent.Add(this, &MapWidget::LayerRemoved);
	}	
}

void MapWidget::LayerAdded(IMapPtr map, ILayerPtr layer, size_t)
{
	if (map->GetLayerCount() == 1)
	{
		_viewPort.SetSpatialReference(map->GetSpatialReference());
	}
}

void MapWidget::LayerRemoved(IMapPtr map, ILayerPtr layer, size_t)
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
	for (vector<IMapTool*>::iterator it = _impl_->_vecMapTool.begin(); it != _impl_->_vecMapTool.end(); ++it)
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
		_impl_->_vecMapTool.push_back(tool);
		MapToolAddedEvent(this, tool);
	}

	return canAdd;
}

bool MapWidget::RemoveMapTool(IMapTool * tool)
{
	for (vector<IMapTool*>::iterator it = _impl_->_vecMapTool.begin(); it != _impl_->_vecMapTool.end(); ++it)
	{
		if (*it == tool)
		{
			tool->SetMapWidget(nullptr);
			_impl_->_vecMapTool.erase(it);
			MapToolRemovedEvent(this, tool);
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

	_geoSurface->SetViewPort(&_viewPort);
	_geoSurface->BeginPaint(false);
	if(map != nullptr)
		map->Paint(_geoSurface);
	_geoSurface->EndPaint(false);
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

	_geoSurface->BeginPaint(true);
	_geoSurface->Present((int)surfPresentLeft, (int)surfPresentTop, surfPresentWidth, surfPresentHeight);
	_geoSurface->EndPaint(true);
}



END_NAME_SPACE(tGis, Core)


