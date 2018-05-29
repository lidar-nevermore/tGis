#pragma once


#ifndef __RASTERRGBLAYER_H__
#define __RASTERRGBLAYER_H__

#include "Helper.h"

#include "RasterLayer.h"

#include <string>

using namespace std;

class GDALRasterBand;

BEGIN_NAME_SPACE(tGis, Core)

class TGIS_API RasterRgbLayer : public RasterLayer
{
public:
	RasterRgbLayer();
	RasterRgbLayer(MyGDALRasterDataset* dataset, int r, int g, int b);
	~RasterRgbLayer();

private:
	RasterRgbLayer(const RasterRgbLayer &) = delete;
	RasterRgbLayer &operator=(const RasterRgbLayer &) = delete;

private:
	static const char* const _type;

public:
	virtual const char* GetType();
	static const char* S_GetType();
	virtual const char* GetCreationString();

public:
	void SetDataset(MyGDALRasterDataset* dataset, int r, int g, int b);
	inline void SetMinMaxR(double min, double max);
	inline void SetMinMaxG(double min, double max);
	inline void SetMinMaxB(double min, double max);
	inline void RestLutToLinear();
	inline unsigned char* GetLutR();
	inline unsigned char* GetLutG();
	inline unsigned char* GetLutB();
	inline int GetBandR();
	inline int GetBandG();
	inline int GetBandB();

protected:
	void OuterResample(unsigned char* pixBuffer, int readingLeft, double alignRmrX, int readingTop, double alignRmrY, int readingWidth, int readingHeight,
		unsigned char* surfBuffer, int paintingLeft, int paintingTop, int paintingWidth, int paintingHeight);

	void IOResample(unsigned char* pixBuffer, int readingLeft, int readingTop, int readingRight, int readingBottom,
		unsigned char* surfBuffer, int paintingLeft, int paintingTop, int paintingWidth, int paintingHeight);

private:
	GDALRasterBand* _rBand;
	int _rBandIndex;
	int _rDataType;
	int _rDataBytes;

	unsigned char _rLut[256];
	double _rMin;
	double _rMax;
	double _rRange;

	GDALRasterBand* _gBand;
	int _gBandIndex;
	int _gDataType;
	int _gDataBytes;

	unsigned char _gLut[256];
	double _gMin;
	double _gMax;
	double _gRange;

	GDALRasterBand* _bBand;
	int _bBandIndex;
	int _bDataType;
	int _bDataBytes;

	unsigned char _bLut[256];
	double _bMin;
	double _bMax;
	double _bRange;
};

END_NAME_SPACE(tGis, Core)

#endif

