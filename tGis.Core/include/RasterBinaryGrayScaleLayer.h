#pragma once
#pragma once


#ifndef __RASTERBINARYGRAYSCALELAYER_H__
#define __RASTERBINARYGRAYSCALELAYER_H__

#include "Helper.h"

#include "RasterLayer.h"


class GDALRasterBand;

BEGIN_NAME_SPACE(tGis, Core)


class TGIS_API RasterBinaryGrayScaleLayer : public RasterLayer
{
	friend class RasterBinaryGrayScaleLayerProvider;
protected:
	RasterBinaryGrayScaleLayer(ILayerProvider* provider);
	RasterBinaryGrayScaleLayer(ILayerProvider* provider, MyGDALRasterDataset* dataset, int band);
	
public:
	~RasterBinaryGrayScaleLayer();

private:
	RasterBinaryGrayScaleLayer(const RasterBinaryGrayScaleLayer &) = delete;
	RasterBinaryGrayScaleLayer &operator=(const RasterBinaryGrayScaleLayer &) = delete;

private:
	static const char* const _type;

public:
	virtual const char* GetType();
	static const char* S_GetType();
	virtual const char* GetCreationString();
	virtual ILayer* Clone(IDataset*);

public:
	inline void SetDataset(MyGDALRasterDataset* dataset, int band);
	inline void SetMinPivotMax(double min, double pivot, double max);
	inline void GetMinPivotMax(double* min, double* pivot, double* max);
	inline unsigned char* GetLut();
	inline int GetBand();

	inline void SetLeftChannel(bool r, bool g, bool b);
	inline void GetLeftChannel(bool* r, bool* g, bool* b);
	inline void SetRightChannel(bool r, bool g, bool b);
	inline void GetRightChannel(bool* r, bool* g, bool* b);

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
	bool leftRChannel;
	bool leftGChannel;
	bool leftBChannel;
	bool rightRChannel;
	bool rightGChannel;
	bool rightBChannel;
};



END_NAME_SPACE(tGis, Core)

#endif

