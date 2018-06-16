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

void RasterBinaryGrayScaleLayerProvider::SetCreationUI(const CreationUI ui)
{
	_uiCreation = ui;
}

ILayer * RasterBinaryGrayScaleLayerProvider::UI_CreateLayer(IDataset * dataset)
{
	assert(_uiCreation != nullptr);

	return _uiCreation(this, (MyGDALRasterDataset*)dataset);
}

void RasterBinaryGrayScaleLayerProvider::SetPropertyUI(const PropertyUI ui)
{
	_uiProperty = ui;
}

void RasterBinaryGrayScaleLayerProvider::UI_LayerProperty(ILayer * layer)
{
	assert(_uiProperty != nullptr);

	_uiProperty(this, (RasterBinaryGrayScaleLayer*)layer);
}

ILayer * RasterBinaryGrayScaleLayerProvider::CreateLayer(MyGDALRasterDataset * dataset, int band)
{
	return new RasterBinaryGrayScaleLayer(dataset,band);
}

ILayer * RasterBinaryGrayScaleLayerProvider::CreateLayer(IDataset * dataset, const char * creationString)
{
	return nullptr;
}

void RasterBinaryGrayScaleLayerProvider::ReleaseLayer(ILayer * layer)
{
	delete layer;
}

void RasterBinaryGrayScaleLayerProvider::Release()
{
}



END_NAME_SPACE(tGis, Core)



