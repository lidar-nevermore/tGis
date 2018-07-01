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
struct IDataset;

class TGIS_API LayerProviderRepository
{
private:
	static LayerProviderRepository* _instance;
public:
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

	int GetLayerProviderCountSupportDataset(IDataset* dataset);
	void GetLayerProviderSupportDataset(IDataset* dataset, int count, ILayerProvider** layerProviders);

private:
	vector<ILayerProvider*> _vecLayerProvider;
	map<string, ILayerProvider*> _mapLayerProvider;
};

END_NAME_SPACE(tGis, Core)


#endif


