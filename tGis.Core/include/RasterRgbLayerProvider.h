#pragma once

#ifndef __RASTERRGBLAYERPROVIDER_H__
#define __RASTERRGBLAYERPROVIDER_H__


#include "Helper.h"

#include "ILayerProvider.h"

BEGIN_NAME_SPACE(tGis, Core)

class RasterRgbLayer;
class MyGDALRasterDataset;

class TGIS_API RasterRgbLayerProvider : public ILayerProvider
{
public:
	typedef ILayer*(*CreationUI)(RasterRgbLayerProvider*, MyGDALRasterDataset*);
	typedef void(*PropertyUI)(RasterRgbLayerProvider*, RasterRgbLayer*);

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
	void SetCreationUI(const CreationUI ui);
	ILayer* UI_CreateLayer(IDataset* dataset);
	void SetPropertyUI(const PropertyUI ui);
	void UI_LayerProperty(ILayer* layer);
	ILayer* CreateLayer(MyGDALRasterDataset* dataset, int r, int g, int b);
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

