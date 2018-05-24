#pragma once

#ifndef __RASTERGRAYSCALELAYERPROVIDER_H__
#define __RASTERGRAYSCALELAYERPROVIDER_H__


#include "Helper.h"

#include "ILayerProvider.h"

BEGIN_NAME_SPACE(tGis, Core)

class TGISCORE_API RasterGrayScaleLayer;

class RasterGrayScaleLayerProvider : public ILayerProvider
{
public:
	typedef RasterGrayScaleLayer*(*UI)();

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

	const char* GetSupportedDatasetType();

	void SetCreationUI(const UI ui);
	ILayer* UI_CreateLayer(IDataset* dataset);
	ILayer* CreateLayer(IDataset* dataset, int band);
	ILayer* CreateLayer(IDataset* dataset, const char* creationString);
	void ReleaseLayer(ILayer*);

	void Release();

private:
	UI _ui;

private:
	static const char* const _name;
	static const char* const _type;
};

END_NAME_SPACE(tGis, Core)


#endif

