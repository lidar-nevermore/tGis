#include "VectorLayer.h"

#include "ogrsf_frmts.h"

BEGIN_NAME_SPACE(tGis, Core)

VectorLayer::VectorLayer()
{
	_layer = nullptr;
	_spatialRef = nullptr;
}

VectorLayer::VectorLayer(OGRLayer *layer)
{
	_layer = layer;
	_layer->GetExtent(&_envelope);
	_spatialRef = _layer->GetSpatialRef();
}


VectorLayer::~VectorLayer()
{
}

void VectorLayer::SetOGRLayer(OGRLayer * layer)
{
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



END_NAME_SPACE(tGis, Core)

