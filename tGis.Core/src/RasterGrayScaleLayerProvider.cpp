#include "RasterGrayScaleLayerProvider.h"
#include "RasterGrayScaleLayer.h"
#include "MyGDALRasterDataset.h"
#include "LayerProviderRepository.h"

#include <cassert>

BEGIN_NAME_SPACE(tGis, Core)

const char* const RasterGrayScaleLayerProvider::_name = "RasterGrayScaleLayerProvider";
const char* const RasterGrayScaleLayerProvider::_type = "FC781B6F-D17D-4B06-8F02-37C1A39C1B3C";
RasterGrayScaleLayerProvider RasterGrayScaleLayerProvider::INSTANCE;

RasterGrayScaleLayerProvider::RasterGrayScaleLayerProvider()
{
	LayerProviderRepository::INSTANCE().AddLayerProvider(this);
}


RasterGrayScaleLayerProvider::~RasterGrayScaleLayerProvider()
{
}

const char * RasterGrayScaleLayerProvider::GetType()
{
	return _type;
}

const char * RasterGrayScaleLayerProvider::GetName()
{
	return _name;
}

const char * RasterGrayScaleLayerProvider::GetSupportedLayerType()
{
	return RasterGrayScaleLayer::S_GetType();
}

bool RasterGrayScaleLayerProvider::IsSupportDataset(IDataset * dataset)
{
	return dataset->IsTypeOf(MyGDALRasterDataset::S_GetType());
}

ILayer * RasterGrayScaleLayerProvider::CreateLayer(MyGDALRasterDataset * dataset, int band)
{
	return new RasterGrayScaleLayer(dataset,band);
}


END_NAME_SPACE(tGis, Core)

