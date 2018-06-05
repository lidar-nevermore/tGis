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

const char * RasterGrayScaleLayerProvider::GetSupportedDatasetType()
{
	return MyGDALRasterDataset::S_GetType();
}

void RasterGrayScaleLayerProvider::SetCreationUI(const CreationUI ui)
{
	_uiCreation = ui;
}

ILayer * RasterGrayScaleLayerProvider::UI_CreateLayer(IDataset * dataset)
{
	assert(_uiCreation != nullptr);

	return _uiCreation(this, (MyGDALRasterDataset*)dataset);
}

void RasterGrayScaleLayerProvider::SetPropertyUI(const PropertyUI ui)
{
	_uiProperty = ui;
}

void RasterGrayScaleLayerProvider::UI_LayerProperty(ILayer * layer)
{
	assert(_uiProperty != nullptr);

	_uiProperty(this, (RasterGrayScaleLayer*)layer);
}

ILayer * RasterGrayScaleLayerProvider::CreateLayer(MyGDALRasterDataset * dataset, int band)
{
	return new RasterGrayScaleLayer(dataset,band);
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

