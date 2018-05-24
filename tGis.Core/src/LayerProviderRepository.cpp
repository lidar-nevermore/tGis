#include "LayerProviderRepository.h"

BEGIN_NAME_SPACE(tGis, Core)

int LayerProviderRepository::GetLayerProviderCount()
{
	return 0;
}

ILayerProvider * LayerProviderRepository::GetLayerProvider(int)
{
	return nullptr;
}

void LayerProviderRepository::AddLayerProvider(ILayerProvider *)
{
}

ILayerProvider * LayerProviderRepository::GetLayerProvider(const char * layerType)
{
	return nullptr;
}

int LayerProviderRepository::GetLayerProviderCountSupportDataset(const char * datasetType)
{
	return 0;
}

void LayerProviderRepository::GetLayerProviderSupportDataset(const char * datasetType, int count, ILayerProvider ** layerProviders)
{
}

LayerProviderRepository::LayerProviderRepository()
{
}


LayerProviderRepository::~LayerProviderRepository()
{
}

END_NAME_SPACE(tGis, Core)

