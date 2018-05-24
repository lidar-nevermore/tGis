#include "LayerProviderRepository.h"
#include "ILayerProvider.h"
#include "RasterGrayScaleLayerProvider.h"
#include "VectorSimpleLayerProvider.h"

BEGIN_NAME_SPACE(tGis, Core)

LayerProviderRepository LayerProviderRepository::INSTANCE;


LayerProviderRepository::LayerProviderRepository()
{
}


LayerProviderRepository::~LayerProviderRepository()
{
	for (vector<ILayerProvider*>::iterator it = _vecLayerProvider.begin(); it != _vecLayerProvider.end(); it++)
	{
		ILayerProvider* lp = *it;

		if (lp != &RasterGrayScaleLayerProvider::INSTANCE && lp != &VectorSimpleLayerProvider::INSTANCE)
			lp->Release();
	}
}

int LayerProviderRepository::GetLayerProviderCount()
{
	return _vecLayerProvider.size();
}

ILayerProvider * LayerProviderRepository::GetLayerProvider(int pos)
{
	return _vecLayerProvider.at(pos);
}

void LayerProviderRepository::AddLayerProvider(ILayerProvider * lp)
{
	string layerType = lp->GetSupportedLayerType();
	string datasetType = lp->GetSupportedDatasetType();
	_vecLayerProvider.push_back(lp);
	_mapLayerProvider.insert(map<string, ILayerProvider*>::value_type(layerType, lp));
	_mapLayerProviderSupportDataset.insert(make_pair(datasetType, lp));
}

ILayerProvider * LayerProviderRepository::GetLayerProvider(const char * layerType)
{
	map<string, ILayerProvider*>::iterator pos = _mapLayerProvider.find(layerType);

	if (pos != _mapLayerProvider.end())
		return (*pos).second;
	return nullptr;
}

int LayerProviderRepository::GetLayerProviderCountSupportDataset(const char * datasetType)
{
	int count = 0;
	pair<multimap<string, ILayerProvider*>::iterator, multimap<string, ILayerProvider*>::iterator> 
		range = _mapLayerProviderSupportDataset.equal_range(datasetType);
	for (multimap<string, ILayerProvider*>::iterator it = range.first; it != range.second; ++it)
	{
		count++;
	}
	return count;
}

void LayerProviderRepository::GetLayerProviderSupportDataset(const char * datasetType, int count, ILayerProvider ** layerProviders)
{
	int i = 0;

	memset(layerProviders, 0, count * sizeof(ILayerProvider*));
	pair<multimap<string, ILayerProvider*>::iterator, multimap<string, ILayerProvider*>::iterator>
		range = _mapLayerProviderSupportDataset.equal_range(datasetType);
	for (multimap<string, ILayerProvider*>::iterator it = range.first; it != range.second; ++it)
	{
		layerProviders[i] = (*it).second;
		i++;
		if (i == count) 
		{
			break;
		}
	}
}


END_NAME_SPACE(tGis, Core)

