#pragma once


#ifndef __RASTERGRAYSCALELAYER_H__
#define __RASTERGRAYSCALELAYER_H__

#include "Helper.h"

#include "RasterLayer.h"

class GDALRasterBand;

BEGIN_NAME_SPACE(tGis, Core)

class RasterGrayScaleLayerProvider;

class TGIS_API RasterGrayScaleLayer : public RasterLayer
{
	friend class RasterGrayScaleLayerProvider;
protected:
	RasterGrayScaleLayer(ILayerProvider* provider);
	RasterGrayScaleLayer(ILayerProvider* provider, MyGDALRasterDataset* dataset, int band);
	
public:
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
	virtual ILayer* Clone(IDataset*);

public:
	inline void SetDataset(MyGDALRasterDataset* dataset, int band);
	inline void SetMinMax(double min, double max);
	inline void GetMinMax(double* min, double* max);
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
