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

	//影像可见范围的绘制表面坐标表示
	double _minSurfX;
	double _minSurfY;
	double _maxSurfX;
	double _maxSurfY;

	//影像可见范围的像元坐标表示
	double _minPixX;
	double _minPixY;
	double _maxPixX;
	double _maxPixY;

	//绘制表面像素与影像像元边长的比值
	//也等于一个绘制表面像素相当于多少个影像像元
	//小于1 影像放大  大于1 影像缩小
	double _surfPixRatio;

	//当影像缩放到超过1：1时需要将原始像元读取出来再进行重采样
	//即_surfPixRatio<1时
	bool _outerResample;

	//需要显示的波段中最大像元字节数
	int _maxPixDataBytes;

};

END_NAME_SPACE(tGis, Core)

#endif
