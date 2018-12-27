#pragma once

#ifndef __RASTERBINARYGRAYSCALELAYERPROVIDER_H__
#define __RASTERBINARYGRAYSCALELAYERPROVIDER_H__


#include "Helper.h"

#include "LayerProvider.h"

BEGIN_NAME_SPACE(tGis, Core)

class MyGDALRasterDataset;
class RasterBinaryGrayScaleLayer;

class TGIS_CORE_API RasterBinaryGrayScaleLayerProvider : public LayerProvider
{
private:
	static const char* const _name;
	static const char* const _type;

public:
	static RasterBinaryGrayScaleLayerProvider INSTANCE;

private:
	RasterBinaryGrayScaleLayerProvider();
	~RasterBinaryGrayScaleLayerProvider();
	RasterBinaryGrayScaleLayerProvider(const RasterBinaryGrayScaleLayerProvider &) = delete;
	RasterBinaryGrayScaleLayerProvider &operator=(const RasterBinaryGrayScaleLayerProvider &) = delete;

public:
	const char* GetType();
	const char* GetName();

	const char* GetSupportedLayerType();

	bool IsSupportDataset(IDataset* dataset);
	ILayer* CreateLayer(MyGDALRasterDataset* dataset, int band);
};


END_NAME_SPACE(tGis, Core)


#endif

