#pragma once

#ifndef __RASTERLAYER_H__
#define __RASTERLAYER_H__

#include "Helper.h"

#include "Layer.h"

#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

class MyGDALRasterDataset;

class TGISCORE_API RasterLayer : public Layer
{
public:
	RasterLayer();
	RasterLayer(MyGDALRasterDataset* raster);
	virtual ~RasterLayer();

public:
	virtual const OGREnvelope* GetEnvelope();
	virtual const OGRSpatialReference* GetSpatialReference();
	virtual IDataset* GetDataset();

protected:
	inline void SetDataset(MyGDALRasterDataset* raster);

protected:
	inline bool PreparePaint(IGeoSurface*);
	inline void SetBufferAlpha(unsigned char* buf, int width, int height);
	
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

};

END_NAME_SPACE(tGis, Core)

#endif
