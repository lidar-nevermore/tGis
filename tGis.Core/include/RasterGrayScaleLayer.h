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

class TGIS_API RasterGrayScaleLayer : public RasterLayer
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
	static const char* S_GetType();
	virtual const char* GetCreationString();

public:
	void SetDataset(MyGDALRasterDataset* dataset, int band);
	inline void SetMinMax(double min, double max);
	inline void GetMinMax(double* min, double* max);
	inline void RestLutToLinear();
	inline unsigned char* GetLut();
	inline int GetBand();

protected:
	void OuterResample(unsigned char* pixBuffer, int readingLeft, double alignRmrX, int readingTop, double alignRmrY, int readingWidth, int readingHeight,
		unsigned char* surfBuffer, int paintingLeft, int paintingTop, int paintingWidth, int paintingHeight);

	void IOResample(unsigned char* pixBuffer, int readingLeft, int readingTop, int readingRight, int readingBottom,
		unsigned char* surfBuffer, int paintingLeft, int paintingTop, int paintingWidth, int paintingHeight);

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
