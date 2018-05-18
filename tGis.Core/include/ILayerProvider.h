#pragma once

#ifndef __I_LAYERPROVIDER_H__
#define __I_LAYERPROVIDER_H__


#include "Helper.h"

BEGIN_NAME_SPACE(tGis, Core)

struct IDataset;
struct ILayer;

struct TGISCORE_API ILayerProvider
{
	virtual const char* GetSupportedDatasetType() = 0;
	virtual const char* GetSupportedLayerType() = 0;
	virtual char* GetName() = 0;

	virtual ILayer* UI_CreateLayer() = 0;
	virtual ILayer* CreateLayer(const IDataset*) = 0;

	virtual ~ILayerProvider() {};
};


END_NAME_SPACE(tGis, Core)


#endif
