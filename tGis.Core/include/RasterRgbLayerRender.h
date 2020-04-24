#pragma once


#ifndef __RASTERRGBLAYERRENDER_H__
#define __RASTERRGBLAYERRENDER_H__

#include "Helper.h"

#include "RasterLayerRender.h"

class GDALRasterBand;

BEGIN_NAME_SPACE(tGis, Core)

class TGIS_CORE_API RasterRgbLayerRender : public RasterLayerRender
{
public:
	virtual const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);

private:
	static const char* const _type;

public:
	RasterRgbLayerRender(ILayer* layer);
	RasterRgbLayerRender(ILayer* layer, int r, int g, int b);
	~RasterRgbLayerRender();

private:
	RasterRgbLayerRender(const RasterRgbLayerRender &) = delete;
	RasterRgbLayerRender &operator=(const RasterRgbLayerRender &) = delete;

public:
	void SetMinMaxR(double min, double max);
	void SetMinMaxG(double min, double max);
	void SetMinMaxB(double min, double max);
	void GetMinMaxR(double* min, double* max);
	void GetMinMaxG(double* min, double* max);
	void GetMinMaxB(double* min, double* max);

	unsigned char* GetLutR();
	unsigned char* GetLutG();
	unsigned char* GetLutB();

	int GetBandR();
	int GetBandG();
	int GetBandB();

	void SetBandR(int rBand);
	void SetBandG(int gBand);
	void SetBandB(int bBand);

	void SetNoDataValueR(int noDataLogic, double noDataValue);
	void GetNoDataValueR(int* noDataLogic, double* noDataValue);
	void SetNoDataValueG(int noDataLogic, double noDataValue);
	void GetNoDataValueG(int* noDataLogic, double* noDataValue);
	void SetNoDataValueB(int noDataLogic, double noDataValue);
	void GetNoDataValueB(int* noDataLogic, double* noDataValue);

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

	int _rNoDataLogic;
	//无效值
	double _rNoDataValue;

	GDALRasterBand* _gBand;
	int _gBandIndex;
	int _gDataType;
	int _gDataBytes;

	unsigned char _gLut[256];
	double _gMin;
	double _gMax;
	double _gRange;

	int _gNoDataLogic;
	//无效值
	double _gNoDataValue;

	GDALRasterBand* _bBand;
	int _bBandIndex;
	int _bDataType;
	int _bDataBytes;

	unsigned char _bLut[256];
	double _bMin;
	double _bMax;
	double _bRange;

	int _bNoDataLogic;
	//无效值
	double _bNoDataValue;
};

END_NAME_SPACE(tGis, Core)

#endif

