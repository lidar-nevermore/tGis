#pragma once

#ifndef __RASTERRGBLAYERPROVIDER_H__
#define __RASTERRGBLAYERPROVIDER_H__


#include "Helper.h"

#include "LayerProvider.h"

BEGIN_NAME_SPACE(tGis, Core)

class RasterRgbLayer;
class MyGDALRasterDataset;

class TGIS_API RasterRgbLayerProvider : public LayerProvider
{
private:
	static const char* const _name;
	static const char* const _type;

public:
	static RasterRgbLayerProvider INSTANCE;

private:
	RasterRgbLayerProvider();
	~RasterRgbLayerProvider();
	RasterRgbLayerProvider(const RasterRgbLayerProvider &) = delete;
	RasterRgbLayerProvider &operator=(const RasterRgbLayerProvider &) = delete;

public:
	const char* GetType();
	const char* GetName();

	const char* GetSupportedLayerType();

	bool IsSupportDataset(IDataset* dataset);
	ILayer* CreateLayer(MyGDALRasterDataset* dataset, int r, int g, int b);
};


END_NAME_SPACE(tGis, Core)


#endif

