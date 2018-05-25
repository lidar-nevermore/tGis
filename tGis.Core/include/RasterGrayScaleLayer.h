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

private:
	RasterGrayScaleLayer(const RasterGrayScaleLayer &) = delete;
	RasterGrayScaleLayer &operator=(const RasterGrayScaleLayer &) = delete;

private:
	static const char* const _type;

public:
	virtual const char* GetType();
	virtual const char* GetCreationString();

public:
	void SetDataset(MyGDALRasterDataset* dataset, int band);
	inline void SetMinMax(double min, double max);
	inline void RestLutToLinear();
	inline unsigned char* GetLut();
	inline int GetBand();

protected:
	void OuterResample(unsigned char* pixBuffer, int readingLeft, int initialReadingLeft, double initialAlignRmrX,int readingTop, int initialReadingTop, double initialAlignRmrY, int readingWidth, int readingHeight,
		                       unsigned char* surfBuffer, int paintingLeft, int initialPaintingLeft, int paintingTop, int initialPaintingTop, int paintingWidth, int paintingHeight) override;

	void IOResample(unsigned char* pixBuffer, int readingLeft, int readingTop, int readingWidth, int readingHeight,
		                    unsigned char* surfBuffer, int paintingLeft, int paintingTop, int paintingWidth, int paintingHeight) override;

private:
	GDALRasterBand* _band;
	int _bandIndex;
	int _dataType;
	int _dataBytes;

	unsigned char _lut[256];
	double _min;
	double _max;
	double _range;
};

END_NAME_SPACE(tGis, Core)

#endif
