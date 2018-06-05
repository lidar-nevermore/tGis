#pragma once

#ifndef __RASTERBINARYGRAYSCALELAYERPROVIDER_H__
#define __RASTERBINARYGRAYSCALELAYERPROVIDER_H__


#include "Helper.h"

#include "ILayerProvider.h"

BEGIN_NAME_SPACE(tGis, Core)

class MyGDALRasterDataset;
class RasterBinaryGrayScaleLayer;

class TGIS_API RasterBinaryGrayScaleLayerProvider : public ILayerProvider
{
public:
	typedef ILayer*(*CreationUI)(RasterBinaryGrayScaleLayerProvider*, MyGDALRasterDataset*);
	typedef void(*PropertyUI)(RasterBinaryGrayScaleLayerProvider*, RasterBinaryGrayScaleLayer*);

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

	const char* GetSupportedDatasetType();

	void SetCreationUI(const CreationUI ui);
	ILayer* UI_CreateLayer(IDataset* dataset);
	void SetPropertyUI(const PropertyUI ui);
	void UI_LayerProperty(ILayer* layer);
	ILayer* CreateLayer(MyGDALRasterDataset* dataset, int band);
	ILayer* CreateLayer(IDataset* dataset, const char* creationString);
	void ReleaseLayer(ILayer*);

	void Release();

private:
	CreationUI _uiCreation;
	PropertyUI _uiProperty;

private:
	static const char* const _name;
	static const char* const _type;
};


END_NAME_SPACE(tGis, Core)


#endif

