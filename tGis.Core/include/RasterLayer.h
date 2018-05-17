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
	virtual void SetName(char* name);
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

	//进行像素对齐操作后 在表面上绘制影像的范围
	int _paintSurfLeft;
	int _paintSurfTop;
	int _paintSurfRight; //不含在内
	int _paintSurfBottom; //不含在内

	//进行像素对齐操作后 在表面上绘制影像的起始点的残差 小于等于 0.5
	double _paintSurfLeftRmr;
	double _paintSurfTopRmr;

	//进行像素对齐操作后 在影像上读取像元的范围 
	int _readPixLeft;
	int _readPixTop;
	int _readPixRight; //不含在内
	int _readPixBottom; //不含在内

	//绘制表面像素与影像像元边长的比值
	//也等于一个绘制表面像素相当于多少个影像像元
	//小于1 影像放大  大于1 影像缩小
	double _surfPixRatio;

	//当影像缩放到超过1：1时需要将原始像元读取出来再进行重采样
	//即_surfPixRatio<1时
	bool _outerResample;

};

END_NAME_SPACE(tGis, Core)