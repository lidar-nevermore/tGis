#pragma once

#ifndef __MAPREPOSITORY_H__
#define __MAPREPOSITORY_H__

#include "Helper.h"

BEGIN_NAME_SPACE(tGis, Core)

struct ILayerProvider;

class TGISCORE_API LayerProviderRepository
{
public:
	static const LayerProviderRepository INSTANCE;

public:
	int GetLayerProviderCount();
	ILayerProvider* GetLayerProvider(int);
	void AddLayerProvider(ILayerProvider*);
	ILayerProvider* GetLayerProvider(const char* layerType);

	int GetLayerProviderCountSupportDataset(const char* datasetType);
	void GetLayerProviderSupportDataset(const char* datasetType, int count, ILayerProvider** layerProviders);

private:
	LayerProviderRepository();
	~LayerProviderRepository();
	LayerProviderRepository(const LayerProviderRepository &) = delete;
	LayerProviderRepository &operator=(const LayerProviderRepository &) = delete;
};

END_NAME_SPACE(tGis, Core)


#endif


