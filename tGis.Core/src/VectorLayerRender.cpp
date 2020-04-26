#include "VectorLayerRender.h"
#include "MyGDALVectorDataset.h"
#include "ITypedObject.h"
#include "IGeoSurface.h"
#include "IMap.h"

#include "ogrsf_frmts.h"

BEGIN_NAME_SPACE(tGis, Core)


const char* const VectorLayerRender::_type = "F5F01BCE-1DAD-44E4-9777-1AA610351E10";


const char * VectorLayerRender::GetType()
{
	return _type;
}

bool VectorLayerRender::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return false;
}

const char * VectorLayerRender::S_GetType()
{
	return _type;
}

VectorLayerRender::VectorLayerRender(ILayer* layer, int ogrLayerIndex)
	:LayerRender(layer)
{
	IDataset* dt = layer->GetDataset();
	if (!dt->IsTypeOf(MyGDALVectorDataset::S_GetType()))
		throw std::exception("图层包含的不是矢量数据集！");


	_vector = (MyGDALVectorDataset*)dt;
	_ogrLayer = _vector->GetGDALDataset()->GetLayer(ogrLayerIndex);
	_ogrLayerIndex = ogrLayerIndex;
	_ogrLayer->GetExtent(&_envelope);
	_spatialRef = _ogrLayer->GetSpatialRef();
	_mapSpatialRef = nullptr;
	_CT = nullptr;
}


VectorLayerRender::~VectorLayerRender()
{
	if (_CT != nullptr)
	{
		OGRCoordinateTransformation::DestroyCT(_CT);
	}
}

const OGREnvelope * VectorLayerRender::GetEnvelope()
{
	PrepareCT();
	return &_envelope;
}

const OGRSpatialReference * VectorLayerRender::GetSpatialReference()
{
	return _spatialRef;
}

bool VectorLayerRender::CanTransformTo(const OGRSpatialReference * spatialRef)
{
	return ILayerRender::CanTransform(_ogrLayer->GetSpatialRef(), spatialRef);
}

inline void VectorLayerRender::PrepareCT()
{
	IMap* map = _layer->GetMap();
	if (map == nullptr)
	{
		return;
	}

	const OGRSpatialReference* mapSpatialRef = map->GetSpatialReference();

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
			_ogrLayer->GetExtent(&envelope);
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

