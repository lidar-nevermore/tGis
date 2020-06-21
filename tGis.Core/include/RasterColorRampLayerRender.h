#pragma once


#ifndef __RasterColorRampLayerRender_H__
#define __RasterColorRampLayerRender_H__

#include "Helper.h"

#include "RasterLayerRender.h"

class GDALRasterBand;

BEGIN_NAME_SPACE(tGis, Core)

class GradientColor;

class TGIS_CORE_API RasterColorRampLayerRender : public RasterLayerRender
{
	TGIS_DECLARE_NO_COPY_CLASS(RasterColorRampLayerRender);

public:
	virtual const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);

private:
	static const char* const _type;

public:
	RasterColorRampLayerRender(ILayer* layer);
	RasterColorRampLayerRender(ILayer* layer, int band);
	~RasterColorRampLayerRender();

public:
	void SetMinMax(double min, double max);
	void GetMinMax(double* min, double* max);

	int GetBand();
	void SetBand(int band);
	void SetNoDataValue(int noDataLogic, double noDataValue);
	void GetNoDataValue(int* noDataLogic, double* noDataValue);

	void SetGradientColor(GradientColor* color);
	GradientColor* GetGradientColor();

public:
	virtual void Paint(IGeoSurface*);

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

	unsigned char _rLut[1000];
	unsigned char _gLut[1000];
	unsigned char _bLut[1000];
	double _min;
	double _max;
	double _range;

	int _noDataLogic;
	//ÎÞÐ§Öµ
	double _noDataValue;

	GradientColor* _color;
};

END_NAME_SPACE(tGis, Core)

#endif
