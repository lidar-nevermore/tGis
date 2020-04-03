#include "Map.h"
#include "ITGisObject.h"
#include "IDataset.h"
#include "ILayerRender.h"

#include "ogr_spatialref.h"

BEGIN_NAME_SPACE(tGis, Core)

Map::Map()
{
	_spatialRef = nullptr;
}


Map::~Map()
{
	ClearLayers();

	if (_spatialRef != nullptr)
	{
		_spatialRef->Dereference();
	}
}

const char * Map::GetName()
{
	return _name.c_str();
}

void Map::SetName(const char * name)
{
	_name = name;
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
	return _vecLayer.size();
}

ILayer * Map::GetLayer(size_t pos)
{
	return _vecLayer.at(pos);
}

size_t Map::GetLayerIndex(ILayer * layer)
{
	size_t i = 0;
	for (vector<ILayer*>::iterator it = _vecLayer.begin(); it != _vecLayer.end(); ++it)
	{
		if (*it == layer)
		{
			break;
		}
		i++;
	}

	return i;
}

size_t Map::AddLayer(ILayer *layer, bool* added)
{
	if (added != nullptr)
		*added = false;

	bool canAdd = false;
	size_t layerCount = _vecLayer.size();
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
		_vecLayer.push_back(layer);
		layer->SetMap(this);
		LayerAddedEvent(std::forward<IMapPtr>(this), std::forward<ILayerPtr>(layer));
		if (added != nullptr)
			*added = true;
	}

	return canAdd ? layerCount : layerCount + 1;
}

ILayer* Map::RemoveLayer(size_t pos)
{
	vector<ILayer*>::iterator it = _vecLayer.begin() + pos;
	ILayer* layer = *it;
	layer->SetMap(nullptr);
	_vecLayer.erase(it);
	MergeEnvelope();
	LayerRemovedEvent(std::forward<IMapPtr>(this), std::forward<ILayerPtr>(layer));
	return layer;
}

void Map::RemoveLayer(ILayer * layer)
{
	for (vector<ILayer*>::iterator it = _vecLayer.begin(); it != _vecLayer.end(); ++it)
	{
		if (*it == layer)
		{
			layer->SetMap(nullptr);
			_vecLayer.erase(it);
			MergeEnvelope();
			LayerRemovedEvent(std::forward<IMapPtr>(this), std::forward<ILayerPtr>(layer));
			break;
		}
	}	
}

void Map::RemoveLayer(IDataset * dt)
{
	for (vector<ILayer*>::iterator it = _vecLayer.begin(); it != _vecLayer.end(); )
	{
		ILayer* layer = *it;

		if (layer->GetDataset() == dt)
		{
			layer->SetMap(nullptr);
			it = _vecLayer.erase(it);
			LayerRemovedEvent(std::forward<IMapPtr>(this), std::forward<ILayerPtr>(layer));
		}
		else
		{
			it++;
		}
	}
	MergeEnvelope();
}

bool Map::InsertLayer(size_t pos, ILayer * layer)
{
	bool canAdd = false;
	OGRSpatialReference* layerSpatialRef = const_cast<OGRSpatialReference*>(layer->GetDataset()->GetSpatialReference());
	if (_vecLayer.size() == 0)
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
		_vecLayer.insert(_vecLayer.begin() + pos, layer);
		layer->SetMap(this);
		LayerAddedEvent(std::forward<IMapPtr>(this), std::forward<ILayerPtr>(layer));
	}

	return canAdd;
}

void Map::MoveLayer(size_t from, size_t to)
{
	if (from < to)
	{
		for (size_t i = from; i < to; i++)
		{
			ILayer* temp = _vecLayer[i];
			_vecLayer[i] = _vecLayer[i + 1];
			_vecLayer[i + 1] = temp;
		}
	}
	else
	{
		for (size_t i = from; i > to; i--)
		{
			ILayer* temp = _vecLayer[i];
			_vecLayer[i] = _vecLayer[i - 1];
			_vecLayer[i - 1] = temp;
		}
	}
}

void Map::ClearLayers()
{
	for (vector<ILayer*>::iterator it = _vecLayer.begin(); it != _vecLayer.end(); it++)
	{
		ILayer* layer = *it;
		if (layer->_is_in_heap)
			delete layer;
	}
	_vecLayer.clear();
	LayerClearedEvent(std::forward<IMapPtr>(this));
}

void Map::Paint(IGeoSurface * surf)
{
	for (vector<ILayer*>::iterator it = _vecLayer.begin(); it != _vecLayer.end(); ++it)
	{
		(*it)->Paint(surf);
	}
}

void Map::MergeEnvelope()
{
	_envelope.MinX = 0.0;
	_envelope.MaxX = 0.0;
	_envelope.MinY = 0.0;
	_envelope.MaxY = 0.0;
	for (vector<ILayer*>::iterator it = _vecLayer.begin(); it != _vecLayer.end(); ++it)
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