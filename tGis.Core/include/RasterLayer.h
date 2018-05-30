#pragma once

#ifndef __RASTERLAYER_H__
#define __RASTERLAYER_H__

#include "Helper.h"

#include "Layer.h"

#include <string>

using namespace std;

class GDALRasterBand;

BEGIN_NAME_SPACE(tGis, Core)

class MyGDALRasterDataset;


class TGIS_API RasterLayer : public Layer
{
public:
	RasterLayer();
	RasterLayer(MyGDALRasterDataset* raster);
	virtual ~RasterLayer();

protected:
	inline void SetDataset(MyGDALRasterDataset* raster);
	inline void InitialMinMax(GDALRasterBand* band, int dataType, double *min, double *max, double* range);

public:
	virtual const OGREnvelope* GetEnvelope();
	virtual const OGRSpatialReference* GetSpatialReference();
	virtual bool CanTransformTo(const OGRSpatialReference*);
	virtual IDataset* GetDataset();

	virtual void Paint(IGeoSurface*);

private:
	inline bool PreparePaint(IGeoSurface*);
	void PaintByOuterResample(IGeoSurface*);
	void PaintByIOResample(IGeoSurface*);

protected:
	inline void SetBufferAlpha(unsigned char* buf, int width, int height);

	typedef void(RasterLayer::*OuterResampleFunc)(unsigned char* pixBuffer, int readingLeft, double alignRmrX, int readingTop, double alignRmrY, int readingWidth, int readingHeight,
		unsigned char* surfBuffer, int paintingLeft, int paintingTop, int paintingWidth, int paintingHeight);

	OuterResampleFunc OuterResample;

	typedef void (RasterLayer::*IOResampleFunc)(unsigned char* pixBuffer, int readingLeft, int readingTop, int readingRight, int readingBottom,
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
	double _surfPixRatio;

	//��Ӱ�����ŵ�����1��1ʱ��Ҫ��ԭʼ��Ԫ��ȡ�����ٽ����ز���
	//��_surfPixRatio<1ʱ
	bool _outerResample;

	//��Ҫ��ʾ�Ĳ����������Ԫ�ֽ���
	int _maxPixDataBytes;

};

END_NAME_SPACE(tGis, Core)

#endif
