#pragma once

#ifndef __LAYERPROVIDERREPOSITORY_H__
#define __LAYERPROVIDERREPOSITORY_H__

#include "Helper.h"

#include <vector>
#include <map>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

struct ILayerProvider;

class TGIS_API LayerProviderRepository
{
public:
	static LayerProviderRepository* _instance;
	static LayerProviderRepository& INSTANCE();

private:
	LayerProviderRepository();
	LayerProviderRepository(const LayerProviderRepository &) = delete;
	LayerProviderRepository &operator=(const LayerProviderRepository &) = delete;

public:
	~LayerProviderRepository();

public:
	int GetLayerProviderCount();
	ILayerProvider* GetLayerProvider(int);
	void AddLayerProvider(ILayerProvider*);
	ILayerProvider* GetLayerProvider(const char* layerType);

	int GetLayerProviderCountSupportDataset(const char* datasetType);
	void GetLayerProviderSupportDataset(const char* datasetType, int count, ILayerProvider** layerProviders);

private:
	vector<ILayerProvider*> _vecLayerProvider;
	map<string, ILayerProvider*> _mapLayerProvider;
	multimap<string, ILayerProvider*> _mapLayerProviderSupportDataset;
};

END_NAME_SPACE(tGis, Core)


#endif


