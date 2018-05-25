#include "VectorLayer.h"
#include "MyGDALVectorDataset.h"

#include "ogrsf_frmts.h"

BEGIN_NAME_SPACE(tGis, Core)

VectorLayer::VectorLayer()
{
	_vector = nullptr;
	_layer = nullptr;
	_spatialRef = nullptr;
}

VectorLayer::VectorLayer(MyGDALVectorDataset* vector, OGRLayer *layer)
{
	_vector = vector;
	_layer = layer;
	_layer->GetExtent(&_envelope);
	_spatialRef = _layer->GetSpatialRef();
}


VectorLayer::~VectorLayer()
{
}

inline void VectorLayer::SetOGRLayer(MyGDALVectorDataset* vector, OGRLayer * layer)
{
	_vector = vector;
	_layer = layer;
	_layer->GetExtent(&_envelope);
	_spatialRef = _layer->GetSpatialRef();
}

const OGREnvelope * VectorLayer::GetEnvelope()
{
	return &_envelope;
}

const OGRSpatialReference * VectorLayer::GetSpatialReference()
{
	return _layer->GetSpatialRef();
}

IDataset * VectorLayer::GetDataset()
{
	return _vector;
}



END_NAME_SPACE(tGis, Core)

