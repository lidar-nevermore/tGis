#include "Map.h"
#include "ogr_spatialref.h"

BEGIN_NAME_SPACE(tGis, Core)

Map::Map()
{
	_spatialRef = nullptr;
}


Map::~Map()
{
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
	const OGRSpatialReference * thisSpatialRef = _spatialRef;
	if (thisSpatialRef == spatialRef)
		return true;

	if (thisSpatialRef != nullptr)
		return thisSpatialRef->IsSame(spatialRef);

	return true;
}

int Map::GetLayerCount()
{
	return _vecLayer.size();
}

ILayer * Map::GetLayer(int pos)
{
	return _vecLayer.at(pos);
}

bool Map::AddLayer(ILayer *layer)
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
	else if(CanTransformFrom(clayerSpatialRef))
	{
		canAdd = true;
		MergeEnvelope(layerSpatialRef, layer->GetEnvelope());
	}

	if(canAdd)
		_vecLayer.insert(_vecLayer.begin(), layer);

	return canAdd;
}

void Map::RemoveLayer(int pos)
{
	_vecLayer.erase(_vecLayer.begin() + pos);
	MergeEnvelope();
}

void Map::RemoveLayer(ILayer * layer)
{
	for (vector<ILayer*>::iterator it = _vecLayer.begin(); it != _vecLayer.end(); ++it)
	{
		if (*it == layer)
		{
			_vecLayer.erase(it);
			MergeEnvelope();
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

	if(canAdd)
		_vecLayer.insert(_vecLayer.begin()+pos, layer);

	return canAdd;
}

void Map::Paint(IGeoSurface * surf)
{
	for (vector<ILayer*>::reverse_iterator it = _vecLayer.rbegin(); it != _vecLayer.rend(); ++it)
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