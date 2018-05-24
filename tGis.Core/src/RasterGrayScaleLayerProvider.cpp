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
	LayerProviderRepository::INSTANCE.AddLayerProvider(this);
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
	return RasterGrayScaleLayer::_type;
}

const char * RasterGrayScaleLayerProvider::GetSupportedDatasetType()
{
	return MyGDALRasterDataset::_type;
}

void RasterGrayScaleLayerProvider::SetCreationUI(const UI ui)
{
	_ui = ui;
}

ILayer * RasterGrayScaleLayerProvider::UI_CreateLayer(IDataset * dataset)
{
	//assert(_ui != nullptr);

	//return _ui();

	return  new RasterGrayScaleLayer((MyGDALRasterDataset*)dataset, 1);
}

ILayer * RasterGrayScaleLayerProvider::CreateLayer(IDataset * dataset, int band)
{
	return new RasterGrayScaleLayer((MyGDALRasterDataset*)dataset,band);
}

ILayer * RasterGrayScaleLayerProvider::CreateLayer(IDataset * dataset, const char * creationString)
{
	return nullptr;
}

void RasterGrayScaleLayerProvider::ReleaseLayer(ILayer * layer)
{
	delete layer;
}

void RasterGrayScaleLayerProvider::Release()
{
}


END_NAME_SPACE(tGis, Core)

