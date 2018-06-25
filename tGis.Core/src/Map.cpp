#include "Map.h"
#include "ITGisObject.h"
#include "ILayerProvider.h"

#include "ogr_spatialref.h"

BEGIN_NAME_SPACE(tGis, Core)

Map::Map()
{
	_spatialRef = nullptr;
}


Map::~Map()
{
	for (vector<ILayer*>::iterator it = _vecLayer.begin(); it != _vecLayer.end(); it++)
	{
		ILayer* layer = *it;
		ILayerProvider* provider = layer->GetProvider();
		provider->ReleaseLayer(layer);
	}

	LayerClearedEvent(std::forward<IMapPtr>(this));

	if (_spatialRef != nullptr)
	{
		_spatialRef->Release();
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

bool Map::CanTransformFrom(const OGRSpatialReference *spatialRef)
{
	return ITGisObject::CanTransform(spatialRef, _spatialRef);
}

int Map::GetLayerCount()
{
	return _vecLayer.size();
}

ILayer * Map::GetLayer(int pos)
{
	return _vecLayer.at(pos);
}

int Map::GetLayerIndex(ILayer * layer)
{
	int i = 0;
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

int Map::AddLayer(ILayer *layer)
{
	bool canAdd = false;
	const OGRSpatialReference* clayerSpatialRef = layer->GetSpatialReference();
	OGRSpatialReference* layerSpatialRef = nullptr;
	if(clayerSpatialRef != nullptr)
		layerSpatialRef = const_cast<OGRSpatialReference*>(clayerSpatialRef);
	if (_vecLayer.size() == 0)
	{
		_spatialRef = layerSpatialRef;
		if(_spatialRef != nullptr)
			_spatialRef->Reference();
		_envelope = *(layer->GetEnvelope());
		canAdd = true;
	}
	else if(layer->CanTransformTo(_spatialRef))
	{
		canAdd = true;
		MergeEnvelope(layerSpatialRef, layer->GetEnvelope());
	}

	if (canAdd)
	{
		_vecLayer.push_back(layer);
		layer->SetMap(this);
		LayerAddedEvent(std::forward<IMapPtr>(this), std::forward<ILayerPtr>(layer));
	}

	return canAdd ? _vecLayer.size() - 1 : -1;
}

ILayer* Map::RemoveLayer(int pos)
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

bool Map::InsertLayer(int pos, ILayer * layer)
{
	bool canAdd = false;
	OGRSpatialReference* layerSpatialRef = const_cast<OGRSpatialReference*>(layer->GetSpatialReference());
	if (_vecLayer.size() == 0)
	{
		_spatialRef = layerSpatialRef;
		if (_spatialRef != nullptr)
			_spatialRef->Reference();
		_envelope = *(layer->GetEnvelope());
		canAdd = true;
	}
	else if (CanTransformFrom(layer->GetSpatialReference()))
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

void Map::MoveLayer(int from, int to)
{
	if (from < to)
	{
		for (int i = from; i < to; i++)
		{
			ILayer* temp = _vecLayer[i];
			_vecLayer[i] = _vecLayer[i + 1];
			_vecLayer[i + 1] = temp;
		}
	}
	else
	{
		for (int i = from; i > to; i--)
		{
			ILayer* temp = _vecLayer[i];
			_vecLayer[i] = _vecLayer[i - 1];
			_vecLayer[i - 1] = temp;
		}
	}
}

void Map::ClearLayers(LayerFunc func)
{
	for (vector<ILayer*>::iterator it = _vecLayer.begin(); it != _vecLayer.end(); it++)
	{
		ILayer* layer = *it;
		
		if (func == nullptr)
		{
			ILayerProvider* provider = layer->GetProvider();
			provider->ReleaseLayer(layer);
		}
		else
		{
			layer->SetMap(nullptr);
			func(layer);
		}
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
		MergeEnvelope((*it)->GetSpatialReference(), (*it)->GetEnvelope());
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