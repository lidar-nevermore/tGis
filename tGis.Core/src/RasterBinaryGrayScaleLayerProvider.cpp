#include "RasterBinaryGrayScaleLayerProvider.h"
#include "RasterBinaryGrayScaleLayer.h"
#include "MyGDALRasterDataset.h"
#include "LayerProviderRepository.h"

#include <cassert>

BEGIN_NAME_SPACE(tGis, Core)

const char* const RasterBinaryGrayScaleLayerProvider::_name = "RasterBinaryGrayScaleLayerProvider";
const char* const RasterBinaryGrayScaleLayerProvider::_type = "B11E3992-4EE0-4D28-9FC4-B0294877B1E4";
RasterBinaryGrayScaleLayerProvider RasterBinaryGrayScaleLayerProvider::INSTANCE;

RasterBinaryGrayScaleLayerProvider::RasterBinaryGrayScaleLayerProvider()
{
	LayerProviderRepository::INSTANCE().AddLayerProvider(this);
}


RasterBinaryGrayScaleLayerProvider::~RasterBinaryGrayScaleLayerProvider()
{
}

const char * RasterBinaryGrayScaleLayerProvider::GetType()
{
	return _type;
}

const char * RasterBinaryGrayScaleLayerProvider::GetName()
{
	return _name;
}

const char * RasterBinaryGrayScaleLayerProvider::GetSupportedLayerType()
{
	return RasterBinaryGrayScaleLayer::S_GetType();
}

bool RasterBinaryGrayScaleLayerProvider::IsSupportDataset(IDataset * dataset)
{
	return dataset->IsTypeOf(MyGDALRasterDataset::S_GetType());
}


ILayer * RasterBinaryGrayScaleLayerProvider::CreateLayer(MyGDALRasterDataset * dataset, int band)
{
	return new RasterBinaryGrayScaleLayer(dataset,band);
}



END_NAME_SPACE(tGis, Core)



