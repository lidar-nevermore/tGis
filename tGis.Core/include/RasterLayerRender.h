#pragma once

#ifndef __RASTERLAYERRENDER_H__
#define __RASTERLAYERRENDER_H__

#include "Helper.h"
#include "LayerRender.h"

class GDALRasterBand;

BEGIN_NAME_SPACE(tGis, Core)

class MyGDALRasterDataset;


class TGIS_CORE_API RasterLayerRender : public LayerRender
{
public:
	virtual const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);

private:
	static const char* const _type;

public:
	static const int EQUAL = 1;
	static const int LT = 2;
	static const int GT = 4;

public:
	RasterLayerRender(ILayer* layer);
	virtual ~RasterLayerRender();

protected:
	void InitialMinMax(GDALRasterBand* band, int dataType, double *min, double *max, double* range);
	void RestLutToLinear(unsigned char lut[256]);
	inline bool IsNoDataValue(int noDataLogic, double noDataValue, double value);

public:
	virtual const OGREnvelope* GetEnvelope();
	virtual bool CanTransformTo(const OGRSpatialReference*);

	virtual void Paint(IGeoSurface*);

private:
    bool PreparePaint(IGeoSurface*);
	void PaintByOuterResample(IGeoSurface*);
	void PaintByIOResample(IGeoSurface*);
	void SetBufferAlpha(unsigned char* buf, int width, int height);

protected:

	typedef void(RasterLayerRender::*OuterResampleFunc)(unsigned char* pixBuffer, int readingLeft, double alignRmrX, int readingTop, double alignRmrY, int readingWidth, int readingHeight,
		unsigned char* surfBuffer, int paintingLeft, int paintingTop, int paintingWidth, int paintingHeight);

	OuterResampleFunc OuterResample;

	typedef void (RasterLayerRender::*IOResampleFunc)(unsigned char* pixBuffer, int readingLeft, int readingTop, int readingRight, int readingBottom,
		unsigned char* surfBuffer, int paintingLeft, int paintingTop, int paintingWidth, int paintingHeight);

	IOResampleFunc IOResample;

protected:
	MyGDALRasterDataset* _raster;

	//Ӱ��ɼ���Χ�Ļ��Ʊ��������ʾ
	double _minSurfX;
	double _minSurfY;
	double _maxSurfX;
	double _maxSurfY;

	//Ӱ��ɼ���Χ����Ԫ�����ʾ
	double _minPixX;
	double _minPixY;
	double _maxPixX;
	double _maxPixY;

	//���Ʊ���������Ӱ����Ԫ�߳��ı�ֵ
	//Ҳ����һ�����Ʊ��������൱�ڶ��ٸ�Ӱ����Ԫ
	//С��1 Ӱ��Ŵ�  ����1 Ӱ����С
	double _surfPixRatioX;
	double _surfPixRatioY;

	//��Ӱ�����ŵ�����1��1ʱ��Ҫ��ԭʼ��Ԫ��ȡ�����ٽ����ز���
	//��_surfPixRatio<1ʱ
	bool _outerResample;

	//��Ҫ��ʾ�Ĳ����������Ԫ�ֽ���
	int _maxPixDataBytes;

};

END_NAME_SPACE(tGis, Core)

#endif
