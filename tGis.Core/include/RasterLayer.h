#pragma once

#include "Helper.h"

#include "ILayer.h"

#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

class MyGDALRasterDataset;

class TGISCORE_API RasterLayer : public ILayer
{
public:
	RasterLayer();
	RasterLayer(MyGDALRasterDataset* dataset);
	virtual ~RasterLayer();

protected:
	void SetDataset(MyGDALRasterDataset* dataset);

public:

	virtual const char* GetName();
	virtual void SetName(const char* name);
	virtual const OGREnvelope* GetEnvelope();
	virtual const OGRSpatialReference* GetSpatialReference();
	virtual bool CanTransformTo(const OGRSpatialReference*);
	virtual bool GetVisible();
	virtual void SetVisible(bool);
	virtual float GetOpacity();
	virtual void SetOpacity(float);
	virtual IDataset* GetDataset(int);

protected:
	bool PreparePaint(IGeoSurface*);
	
protected:
	MyGDALRasterDataset* _dataset;

	string _name;
	bool _visible;
	float _opacity;
	unsigned char _alpha;

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