#pragma once
#pragma once


#ifndef __RASTERBINARYGRAYSCALELAYER_H__
#define __RASTERBINARYGRAYSCALELAYER_H__

#include "Helper.h"

#include "RasterLayerRender.h"


class GDALRasterBand;

BEGIN_NAME_SPACE(tGis, Core)


class TGIS_CORE_API RasterBinaryGrayScaleLayer : public RasterLayerRender
{
public:
	RasterBinaryGrayScaleLayer(ILayer* layer, int band);
	~RasterBinaryGrayScaleLayer();

private:
	RasterBinaryGrayScaleLayer(const RasterBinaryGrayScaleLayer &) = delete;
	RasterBinaryGrayScaleLayer &operator=(const RasterBinaryGrayScaleLayer &) = delete;

private:
	static const char* const _type;

public:
	virtual const char* GetType();
	static const char* S_GetType();

public:
	void SetMinPivotMax(double min, double pivot, double max);
	void GetMinPivotMax(double* min, double* pivot, double* max);
	unsigned char* GetLut();
	int GetBand();

	void SetLeftChannel(bool r, bool g, bool b);
	void GetLeftChannel(bool* r, bool* g, bool* b);
	void SetRightChannel(bool r, bool g, bool b);
	void GetRightChannel(bool* r, bool* g, bool* b);

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
	double _pivot;
	double _range;
	double _leftRange;
	double _rightRange;
	bool _leftRChannel;
	bool _leftGChannel;
	bool _leftBChannel;
	bool _rightRChannel;
	bool _rightGChannel;
	bool _rightBChannel;

	int _noDataLogic;
	//ÎÞÐ§Öµ
	double _noDataValue;
};



END_NAME_SPACE(tGis, Core)

#endif

