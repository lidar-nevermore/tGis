#pragma once


#ifndef __RASTERGRAYSCALELAYER_H__
#define __RASTERGRAYSCALELAYER_H__

#include "Helper.h"

#include "RasterLayer.h"

#include <string>

using namespace std;

class GDALRasterBand;

BEGIN_NAME_SPACE(tGis, Core)

class RasterGrayScaleLayerProvider;

class TGISCORE_API RasterGrayScaleLayer : public RasterLayer
{
	friend class RasterGrayScaleLayerProvider;
public:
	RasterGrayScaleLayer();
	RasterGrayScaleLayer(MyGDALRasterDataset* dataset, int band);
	~RasterGrayScaleLayer();
	void SetDataset(MyGDALRasterDataset* dataset, int band);

private:
	RasterGrayScaleLayer(const RasterGrayScaleLayer &) = delete;
	RasterGrayScaleLayer &operator=(const RasterGrayScaleLayer &) = delete;

private:
	using RasterLayer::SetDataset;

private:
	static const char* const _type;

public:
	virtual const char* GetType();
	virtual const char* GetCreationString();
	virtual void SetOpacity(float);

	virtual void Paint(IGeoSurface*);

private:
	virtual void PaintByOuterResample(IGeoSurface*);
	virtual void PaintByIOResample(IGeoSurface*);

private:
	const static int _bufferAreaWidth = 352;

	//_bufferAreaWidth*_bufferAreaWidth ARGB
	unsigned char* _surfBuffer;
	//_bufferAreaWidth*_bufferAreaWidth GRAY
	unsigned char* _pixBuffer;

	GDALRasterBand* _band;
	int _bandIndex;

};

END_NAME_SPACE(tGis, Core)

#endif
