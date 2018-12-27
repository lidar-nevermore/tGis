#pragma once

#ifndef __RASTERGRAYSCALELAYERPROVIDER_H__
#define __RASTERGRAYSCALELAYERPROVIDER_H__


#include "Helper.h"

#include "LayerProvider.h"

BEGIN_NAME_SPACE(tGis, Core)

class RasterGrayScaleLayer;
class MyGDALRasterDataset;

class TGIS_CORE_API RasterGrayScaleLayerProvider : public LayerProvider
{
private:
	static const char* const _name;
	static const char* const _type;

public:
	static RasterGrayScaleLayerProvider INSTANCE;

private:
	RasterGrayScaleLayerProvider();
	~RasterGrayScaleLayerProvider();
	RasterGrayScaleLayerProvider(const RasterGrayScaleLayerProvider &) = delete;
	RasterGrayScaleLayerProvider &operator=(const RasterGrayScaleLayerProvider &) = delete;

public:
	const char* GetType();
	const char* GetName();

	const char* GetSupportedLayerType();

	bool IsSupportDataset(IDataset* dataset);

	ILayer* CreateLayer(MyGDALRasterDataset* dataset, int band);
};

END_NAME_SPACE(tGis, Core)


#endif

