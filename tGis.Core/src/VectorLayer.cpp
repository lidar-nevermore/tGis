#include "VectorLayer.h"
#include "MyGDALVectorDataset.h"
#include "ITGisObject.h"
#include "IGeoSurface.h"
#include "IMap.h"

#include "ogrsf_frmts.h"

BEGIN_NAME_SPACE(tGis, Core)

VectorLayer::VectorLayer(ILayerProvider* provider)
	:Layer(provider)
{
	_vector = nullptr;
	_layer = nullptr;
	_spatialRef = nullptr;
	_mapSpatialRef = nullptr;
	_CT = nullptr;
}

VectorLayer::VectorLayer(ILayerProvider* provider, MyGDALVectorDataset* vector, OGRLayer *layer)
	:Layer(provider)
{
	_vector = vector;
	_layer = layer;
	_layer->GetExtent(&_envelope);
	_spatialRef = _layer->GetSpatialRef();
	_mapSpatialRef = nullptr;
	_CT = nullptr;
}


VectorLayer::~VectorLayer()
{
	if (_CT != nullptr)
	{
		OGRCoordinateTransformation::DestroyCT(_CT);
	}
}

void VectorLayer::SetOGRLayer(MyGDALVectorDataset* vector, OGRLayer * layer)
{
	_vector = vector;
	_layer = layer;
	_layer->GetExtent(&_envelope);
	_spatialRef = _layer->GetSpatialRef();
}

const OGREnvelope * VectorLayer::GetEnvelope()
{
	PrepareCT();
	return &_envelope;
}

const OGRSpatialReference * VectorLayer::GetSpatialReference()
{
	return _layer->GetSpatialRef();
}

bool VectorLayer::CanTransformTo(const OGRSpatialReference * spatialRef)
{
	return ITGisObject::CanTransform(_layer->GetSpatialRef(), spatialRef);
}

IDataset * VectorLayer::GetDataset()
{
	return _vector;
}

inline void VectorLayer::PrepareCT()
{
	if (_map == nullptr)
	{
		_layer->GetExtent(&_envelope);
		return;
	}

	const OGRSpatialReference* mapSpatialRef = _map->GetSpatialReference();

	if (_spatialRef != nullptr 
		&& mapSpatialRef != nullptr 
		&& _mapSpatialRef != mapSpatialRef
		&&!(_mapSpatialRef != nullptr && mapSpatialRef->IsSame(_mapSpatialRef)))
	{
		if (_CT != nullptr)
		{
			OGRCoordinateTransformation::DestroyCT(_CT);
		}

		_CT = OGRCreateCoordinateTransformation(_spatialRef, const_cast<OGRSpatialReference*>(mapSpatialRef));

		if (_CT != nullptr)
		{
			OGREnvelope envelope;
			_layer->GetExtent(&envelope);
			double x[2] = { envelope.MinX, envelope.MaxX };
			double y[2] = { envelope.MinY, envelope.MaxY };
			_CT->Transform(2, x, y);
			_envelope.MinX = 0.0;
			_envelope.MaxX = 0.0;
			_envelope.MinY = 0.0;
			_envelope.MaxY = 0.0;
			_envelope.Merge(x[0], y[0]);
			_envelope.Merge(x[1], y[1]);
		}
	}

	_mapSpatialRef = const_cast<OGRSpatialReference*>(mapSpatialRef);
}



END_NAME_SPACE(tGis, Core)

