#pragma once


#ifndef __RasterPalettedLayerRender_H__
#define __RasterPalettedLayerRender_H__

#include "Helper.h"

#include "RasterLayerRender.h"

class GDALRasterBand;

BEGIN_NAME_SPACE(tGis, Core)

class Palette;

class TGIS_CORE_API RasterPalettedLayerRender : public RasterLayerRender
{
	TGIS_DECLARE_NO_COPY_CLASS(RasterPalettedLayerRender);

public:
	virtual const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);

private:
	static const char* const _type;

public:
	RasterPalettedLayerRender(ILayer* layer);
	RasterPalettedLayerRender(ILayer* layer, int band);
	~RasterPalettedLayerRender();

public:
	int GetBand();
	void SetBand(int band);
	void SetNoDataValue(int noDataLogic, double noDataValue);
	void GetNoDataValue(int* noDataLogic, double* noDataValue);

	void SetPalette(Palette* color);
	Palette* GetPalette();

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

	int _noDataLogic;
	//ÎÞÐ§Öµ
	double _noDataValue;

	Palette* _color;
};

END_NAME_SPACE(tGis, Core)

#endif
