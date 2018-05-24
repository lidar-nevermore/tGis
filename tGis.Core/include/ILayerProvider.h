#pragma once

#ifndef __I_LAYERPROVIDER_H__
#define __I_LAYERPROVIDER_H__


#include "Helper.h"

BEGIN_NAME_SPACE(tGis, Core)

struct IDataset;
struct ILayer;

struct TGISCORE_API ILayerProvider
{
	virtual const char* GetType() = 0;
	virtual const char* GetName() = 0;

	virtual const char* GetSupportedLayerType() = 0;

	virtual const char* GetSupportedDatasetType() = 0;

	virtual ILayer* UI_CreateLayer(IDataset* dataset) = 0;
	virtual ILayer* CreateLayer(IDataset* dataset, const char* creationString) = 0;
	virtual void ReleaseLayer(ILayer*) = 0;

	virtual void Release() = 0;

	ILayerProvider() {};
	virtual ~ILayerProvider() {};
private:
	ILayerProvider(const ILayerProvider &) = delete;
	ILayerProvider &operator=(const ILayerProvider &) = delete;
};

typedef ILayerProvider* ILayerProviderPtr;

END_NAME_SPACE(tGis, Core)


#endif
