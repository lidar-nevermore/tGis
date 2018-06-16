#include "RasterRgbLayerProvider.h"
#include "RasterRgbLayer.h"
#include "LayerProviderRepository.h"
#include "MyGDALRasterDataset.h"

#include <cassert>

BEGIN_NAME_SPACE(tGis, Core)

const char* const RasterRgbLayerProvider::_name = "RasterRgbLayerProvider";
const char* const RasterRgbLayerProvider::_type = "FC781B6F-D17D-4B06-8F02-37C1A39C1B3C";
RasterRgbLayerProvider RasterRgbLayerProvider::INSTANCE;

RasterRgbLayerProvider::RasterRgbLayerProvider()
{
	LayerProviderRepository::INSTANCE().AddLayerProvider(this);
}


RasterRgbLayerProvider::~RasterRgbLayerProvider()
{
}

const char * RasterRgbLayerProvider::GetType()
{
	return _type;
}

const char * RasterRgbLayerProvider::GetName()
{
	return _name;
}

const char * RasterRgbLayerProvider::GetSupportedLayerType()
{
	return RasterRgbLayer::S_GetType();
}

bool RasterRgbLayerProvider::IsSupportDataset(IDataset * dataset)
{
	return dataset->IsTypeOf(MyGDALRasterDataset::S_GetType());
}

ILayer * RasterRgbLayerProvider::CreateLayer(MyGDALRasterDataset * dataset, int r, int g, int b)
{
	return new RasterRgbLayer(dataset,r,g,b);
}


END_NAME_SPACE(tGis, Core)


