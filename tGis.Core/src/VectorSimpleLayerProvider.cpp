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
	LayerProviderRepository::INSTANCE.AddLayerProvider(this);
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
	return VectorSimpleLayer::_type;
}

const char * VectorSimpleLayerProvider::GetSupportedDatasetType()
{
	return MyGDALVectorDataset::_type;
}

void VectorSimpleLayerProvider::SetCreationUI(const UI ui)
{
	_ui = ui;
}

ILayer * VectorSimpleLayerProvider::UI_CreateLayer(IDataset * dataset)
{
	assert(_ui != nullptr);

	return _ui();
}

ILayer * VectorSimpleLayerProvider::CreateLayer(MyGDALVectorDataset * vector, OGRLayer * layer, SimpleMarkerSymbol * markerSymbol, SimpleLineSymbol * lineSymbol, SimpleFillSymbol * fillSymbol, int geometryField, int labelField)
{
	return nullptr;
}

ILayer * VectorSimpleLayerProvider::CreateLayer(IDataset * dataset, const char * creationString)
{
	return nullptr;
}

void VectorSimpleLayerProvider::ReleaseLayer(ILayer *)
{
}

void VectorSimpleLayerProvider::Release()
{
}

END_NAME_SPACE(tGis, Core)

