#pragma once

#ifndef __RASTERGRAYSCALELAYERPROVIDER_H__
#define __RASTERGRAYSCALELAYERPROVIDER_H__


#include "Helper.h"

#include "ILayerProvider.h"

BEGIN_NAME_SPACE(tGis, Core)

class RasterGrayScaleLayer;
class MyGDALRasterDataset;

class TGIS_API RasterGrayScaleLayerProvider : public ILayerProvider
{
public:
	typedef ILayer*(*CreationUI)(RasterGrayScaleLayerProvider*, MyGDALRasterDataset*);
	typedef void(*PropertyUI)(RasterGrayScaleLayerProvider*, RasterGrayScaleLayer*);

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

