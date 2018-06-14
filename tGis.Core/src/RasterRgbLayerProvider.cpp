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

const char * RasterRgbLayerProvider::GetSupportedDatasetType()
{
	return MyGDALRasterDataset::S_GetType();
}

void RasterRgbLayerProvider::SetCreationUI(const CreationUI ui)
{
	_uiCreation = ui;
}

ILayer * RasterRgbLayerProvider::UI_CreateLayer(IDataset * dataset)
{
	assert(_uiCreation != nullptr);

	return _uiCreation(this, (MyGDALRasterDataset*)dataset);
}

void RasterRgbLayerProvider::SetPropertyUI(const PropertyUI ui)
{
	_uiProperty = ui;
}

void RasterRgbLayerProvider::UI_LayerProperty(ILayer * layer)
{
	assert(_uiProperty != nullptr);

	_uiProperty(this, (RasterRgbLayer*)layer);
}

ILayer * RasterRgbLayerProvider::CreateLayer(MyGDALRasterDataset * dataset, int r, int g, int b)
{
	return new RasterRgbLayer(dataset,r,g,b);
}

ILayer * RasterRgbLayerProvider::CreateLayer(IDataset * dataset, const char * creationString)
{
	return nullptr;
}

void RasterRgbLayerProvider::ReleaseLayer(ILayer * layer)
{
	delete layer;
}

void RasterRgbLayerProvider::Release()
{
}



END_NAME_SPACE(tGis, Core)


