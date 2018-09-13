#include "LayerProviderRepository.h"
#include "ILayerProvider.h"
#include "RasterGrayScaleLayerProvider.h"
#include "RasterBinaryGrayScaleLayerProvider.h"
#include "VectorSimpleLayerProvider.h"
#include "RasterRgbLayerProvider.h"

BEGIN_NAME_SPACE(tGis, Core)

LayerProviderRepository* LayerProviderRepository::_instance = nullptr;


LayerProviderRepository & LayerProviderRepository::INSTANCE()
{
	if (_instance == nullptr)
	{
		_instance = new LayerProviderRepository();
		static _tGisObjectDestructor<LayerProviderRepository> shit(_instance);
	}

	return *_instance;
}

LayerProviderRepository::LayerProviderRepository()
{
}


LayerProviderRepository::~LayerProviderRepository()
{
	for (vector<ILayerProvider*>::iterator it = _vecLayerProvider.begin(); it != _vecLayerProvider.end(); it++)
	{
		ILayerProvider* lp = *it;

		if (lp != &RasterGrayScaleLayerProvider::INSTANCE 
			&& lp != &VectorSimpleLayerProvider::INSTANCE
			&& lp != &RasterBinaryGrayScaleLayerProvider::INSTANCE
			&& lp != &RasterRgbLayerProvider::INSTANCE)
			lp->Release();
	}
}

size_t LayerProviderRepository::GetLayerProviderCount()
{
	return _vecLayerProvider.size();
}

ILayerProvider * LayerProviderRepository::GetLayerProvider(size_t pos)
{
	return _vecLayerProvider.at(pos);
}

void LayerProviderRepository::AddLayerProvider(ILayerProvider * lp)
{
	string layerType = lp->GetSupportedLayerType();
	_vecLayerProvider.push_back(lp);
	_mapLayerProvider.insert(map<string, ILayerProvider*>::value_type(layerType, lp));
}

ILayerProvider * LayerProviderRepository::GetLayerProvider(const char * layerType)
{
	map<string, ILayerProvider*>::iterator pos = _mapLayerProvider.find(layerType);

	if (pos != _mapLayerProvider.end())
		return (*pos).second;
	return nullptr;
}

int LayerProviderRepository::GetLayerProviderCountSupportDataset(IDataset* dataset)
{
	int count = 0;
	for (vector<ILayerProvider*>::iterator it = _vecLayerProvider.begin(); it != _vecLayerProvider.end(); it++)
	{
		ILayerProvider* lp = *it;

		if (lp->IsSupportDataset(dataset))
			count++;
	}
	return count;
}

void LayerProviderRepository::GetLayerProviderSupportDataset(IDataset* dataset, int count, ILayerProvider ** layerProviders)
{
	int i = 0;

	memset(layerProviders, 0, count * sizeof(ILayerProvider*));
	for (vector<ILayerProvider*>::iterator it = _vecLayerProvider.begin(); it != _vecLayerProvider.end(); it++)
	{
		ILayerProvider* lp = *it;

		if (lp->IsSupportDataset(dataset))
		{
			layerProviders[i] = lp;
			i++;
		}
		if (i == count)
		{
			break;
		}
	}
}


END_NAME_SPACE(tGis, Core)

