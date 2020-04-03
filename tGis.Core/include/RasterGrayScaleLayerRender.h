#pragma once


#ifndef __RASTERGRAYSCALELAYERRENDER_H__
#define __RASTERGRAYSCALELAYERRENDER_H__

#include "Helper.h"

#include "RasterLayerRender.h"

class GDALRasterBand;

BEGIN_NAME_SPACE(tGis, Core)

class TGIS_CORE_API RasterGrayScaleLayerRender : public RasterLayerRender
{
public:
	RasterGrayScaleLayerRender(ILayer* layer, int band);
	~RasterGrayScaleLayerRender();

private:
	RasterGrayScaleLayerRender(const RasterGrayScaleLayerRender &) = delete;
	RasterGrayScaleLayerRender &operator=(const RasterGrayScaleLayerRender &) = delete;

private:
	static const char* const _type;

public:
	virtual const char* GetType();
	static const char* S_GetType();

public:
	void SetMinMax(double min, double max);
	void GetMinMax(double* min, double* max);
	unsigned char* GetLut();
	int GetBand();
	void SetNoDataValue(int noDataLogic, double noDataValue);
	void GetNoDataValue(int* noDataLogic, double* noDataValue);

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

	int _noDataLogic;
	//ÎÞÐ§Öµ
	double _noDataValue;
};

END_NAME_SPACE(tGis, Core)

#endif
