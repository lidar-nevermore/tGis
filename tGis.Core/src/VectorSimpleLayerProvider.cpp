#include "VectorSimpleLayerProvider.h"
#include "VectorSimpleLayer.h"
#include "MyGDALVectorDataset.h"
#include "LayerProviderRepository.h"

#include <cassert>

BEGIN_NAME_SPACE(tGis, Core)

const char* const VectorSimpleLayerProvider::_name = "VectorSimpleLayerProvider";
const char* const VectorSimpleLayerProvider::_type = "7555868B-A0DA-4611-B04E-68102D93DEA2";
VectorSimpleLayerProvider VectorSimpleLayerProvider::INSTANCE;

VectorSimpleLayerProvider::VectorSimpleLayerProvider()
{
	LayerProviderRepository::INSTANCE().AddLayerProvider(this);
}


VectorSimpleLayerProvider::~VectorSimpleLayerProvider()
{
}

const char * VectorSimpleLayerProvider::GetType()
{
	return _type;
}

const char * VectorSimpleLayerProvider::GetName()
{
	return _name;
}

const char * VectorSimpleLayerProvider::GetSupportedLayerType()
{
	return VectorSimpleLayer::S_GetType();
}

bool VectorSimpleLayerProvider::IsSupportDataset(IDataset * dataset)
{
	return dataset->IsTypeOf(MyGDALVectorDataset::S_GetType());
}


ILayer * VectorSimpleLayerProvider::UI_CreateLayer(IDataset * dataset)
{
	MyGDALVectorDataset* vector = (MyGDALVectorDataset*)dataset;
	VectorSimpleLayer* layer = new VectorSimpleLayer(vector, vector->GetGDALDataset()->GetLayer(0));
	layer->SetName(vector->GetName());
	return layer;
}


ILayer * VectorSimpleLayerProvider::CreateLayer(MyGDALVectorDataset * vector, OGRLayer * layer, SimpleMarkerSymbol * markerSymbol, SimpleLineSymbol * lineSymbol, SimpleFillSymbol * fillSymbol, int geometryField, int labelField)
{
	return nullptr;
}


END_NAME_SPACE(tGis, Core)

