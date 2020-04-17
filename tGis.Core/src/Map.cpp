#include "Map.h"
#include "ITypedObject.h"
#include "IDataset.h"
#include "ILayerRender.h"

#include "ogr_spatialref.h"

#include <assert.h>
#include <string>
#include <vector>

using namespace std;


BEGIN_NAME_SPACE(tGis, Core)

class MapImpl
{
public:
	MapImpl(Map* owner)
	{
		_owner = owner;
	}

	Map* _owner;
	vector<ILayer*> _vecLayer;
	string _name;
};

Map::Map()
{
	_impl_ = new MapImpl(this);
	_spatialRef = nullptr;
}


Map::~Map()
{
	for (vector<ILayer*>::iterator it = _impl_->_vecLayer.begin(); it != _impl_->_vecLayer.end(); it++)
	{
		ILayer* layer = *it;
		if (layer->_is_in_heap)
			delete layer;
	}
	_impl_->_vecLayer.clear();

	if (_spatialRef != nullptr)
	{
		_spatialRef->Dereference();
	}
	delete _impl_;
}

const char * Map::GetName()
{
	return _impl_->_name.c_str();
}

void Map::SetName(const char * name)
{
	_impl_->_name = name;
}

const OGREnvelope * Map::GetEnvelope()
{
	return &_envelope;;
}

const OGRSpatialReference * Map::GetSpatialReference()
{
	return _spatialRef;
}

size_t Map::GetLayerCount()
{
	return _impl_->_vecLayer.size();
}

ILayer * Map::GetLayer(size_t pos)
{
	return _impl_->_vecLayer.at(pos);
}

bool Map::AddLayer(ILayer *layer)
{
	bool canAdd = false;
	size_t layerCount = _impl_->_vecLayer.size();
	const OGRSpatialReference* clayerSpatialRef = layer->GetDataset()->GetSpatialReference();
	OGRSpatialReference* layerSpatialRef = nullptr;
	if(clayerSpatialRef != nullptr)
		layerSpatialRef = const_cast<OGRSpatialReference*>(clayerSpatialRef);
	if (layerCount == 0)
	{
		_spatialRef = layerSpatialRef;
		if(_spatialRef != nullptr)
			_spatialRef->Reference();
		_envelope = *(layer->GetEnvelope());
		canAdd = true;
	}
	else if(layer->GetRender()->CanTransformTo(_spatialRef))
	{
		canAdd = true;
		MergeEnvelope(layerSpatialRef, layer->GetEnvelope());
	}

	if (canAdd)
	{
		_impl_->_vecLayer.insert(_impl_->_vecLayer.begin(), layer);
		layer->SetMap(this);
		LayerAddedEvent(std::forward<IMapPtr>(this), std::forward<ILayerPtr>(layer), 0);
	}

	return canAdd;
}

ILayer* Map::RemoveLayer(size_t pos)
{
	vector<ILayer*>::iterator it = _impl_->_vecLayer.begin() + pos;
	ILayer* layer = *it;
	layer->SetMap(nullptr);
	_impl_->_vecLayer.erase(it);
	UpdateEnvelope();
	LayerRemovedEvent(std::forward<IMapPtr>(this), std::forward<ILayerPtr>(layer), pos);
	return layer;
}

void Map::RemoveLayer(ILayer * layer)
{
	for (vector<ILayer*>::iterator it = _impl_->_vecLayer.begin(); it != _impl_->_vecLayer.end(); ++it)
	{
		if (*it == layer)
		{
			layer->SetMap(nullptr);
			size_t pos = it - _impl_->_vecLayer.begin();
			_impl_->_vecLayer.erase(it);
			UpdateEnvelope();
			LayerRemovedEvent(std::forward<IMapPtr>(this), std::forward<ILayerPtr>(layer), pos);
			break;
		}
	}	
}

void Map::RemoveLayer(IDataset * dt)
{
	for (vector<ILayer*>::iterator it = _impl_->_vecLayer.begin(); it != _impl_->_vecLayer.end(); )
	{
		ILayer* layer = *it;

		if (layer->GetDataset() == dt)
		{
			layer->SetMap(nullptr);
			size_t pos = it - _impl_->_vecLayer.begin();
			it = _impl_->_vecLayer.erase(it);
			LayerRemovedEvent(std::forward<IMapPtr>(this), std::forward<ILayerPtr>(layer), pos);
		}
		else
		{
			it++;
		}
	}
	UpdateEnvelope();
}

bool Map::InsertLayer(size_t pos, ILayer * layer)
{
	bool canAdd = false;
	OGRSpatialReference* layerSpatialRef = const_cast<OGRSpatialReference*>(layer->GetDataset()->GetSpatialReference());
	if (_impl_->_vecLayer.size() == 0)
	{
		_spatialRef = layerSpatialRef;
		if (_spatialRef != nullptr)
			_spatialRef->Reference();
		_envelope = *(layer->GetEnvelope());
		canAdd = true;
	}
	else if (layer->GetRender()->CanTransformTo(_spatialRef))
	{
		canAdd = true;
		MergeEnvelope(layerSpatialRef, layer->GetEnvelope());
	}

	if (canAdd)
	{
		_impl_->_vecLayer.insert(_impl_->_vecLayer.begin() + pos, layer);
		layer->SetMap(this);
		LayerAddedEvent(std::forward<IMapPtr>(this), std::forward<ILayerPtr>(layer), pos);
	}

	return canAdd;
}

void Map::MoveLayer(size_t from, size_t to)
{
	assert(from >= 0 && from < _impl_->_vecLayer.size());
	assert(to >= 0 && to < _impl_->_vecLayer.size());

	ILayer* fromLayer = _impl_->_vecLayer[from];
	ILayer* toLayer = _impl_->_vecLayer[to];
	_impl_->_vecLayer[from] = toLayer;
	_impl_->_vecLayer[to] = fromLayer;
}

void Map::ClearLayers()
{
	for (vector<ILayer*>::iterator it = _impl_->_vecLayer.begin(); it != _impl_->_vecLayer.end(); it++)
	{
		ILayer* layer = *it;
		if (layer->_is_in_heap)
			delete layer;
	}
	_impl_->_vecLayer.clear();
	LayerClearedEvent(std::forward<IMapPtr>(this));
}

void Map::Paint(IGeoSurface * surf)
{
	for (vector<ILayer*>::reverse_iterator it = _impl_->_vecLayer.rbegin(); it != _impl_->_vecLayer.rend(); ++it)
	{
		(*it)->Paint(surf);
	}
}

void Map::UpdateEnvelope()
{
	_envelope.MinX = 0.0;
	_envelope.MaxX = 0.0;
	_envelope.MinY = 0.0;
	_envelope.MaxY = 0.0;
	for (vector<ILayer*>::iterator it = _impl_->_vecLayer.begin(); it != _impl_->_vecLayer.end(); ++it)
	{
		MergeEnvelope((*it)->GetDataset()->GetSpatialReference(), (*it)->GetEnvelope());
	}
}

void Map::MergeEnvelope(const OGRSpatialReference* spatialRef, const OGREnvelope * envelope)
{
	if (_spatialRef == spatialRef || (_spatialRef != nullptr && _spatialRef->IsSame(spatialRef)))
	{
		_envelope.Merge(*envelope);
	}
	else
	{
		OGRCoordinateTransformation *poCT = OGRCreateCoordinateTransformation(const_cast<OGRSpatialReference*>(spatialRef), _spatialRef);

		double x[2] = { envelope->MinX, envelope->MaxX };
		double y[2] = { envelope->MinY, envelope->MaxY };

		poCT->Transform(2, x, y);

		_envelope.Merge(x[0], y[0]);
		_envelope.Merge(x[1], y[1]);

		OGRCoordinateTransformation::DestroyCT(poCT);
	}
}

END_NAME_SPACE(tGis, Core)