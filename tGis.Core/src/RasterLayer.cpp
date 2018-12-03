#include "RasterLayer.h"
#include "MyGDALRasterDataset.h"
#include "IGeoSurface.h"
#include "ITGisObject.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include <algorithm>
#include <cmath>

#include "VisualizeBufferManager.h"

BEGIN_NAME_SPACE(tGis, Core)


RasterLayer::RasterLayer(ILayerProvider* provider)
	:Layer(provider)
{
	_visible = true;
	_opacity = 1.0;
	_alpha = 255;
}

RasterLayer::RasterLayer(ILayerProvider* provider, MyGDALRasterDataset* raster)
	:Layer(provider)
{
	_visible = true;
	_opacity = 1.0;
	_alpha = 255;

	if (!raster->IsNorthUp())
	{
		throw std::exception("不支持非北方朝上的影响！");
	}
	_raster = raster;
	_name = raster->GetName();
}


RasterLayer::~RasterLayer()
{
}

const OGREnvelope * RasterLayer::GetEnvelope()
{
	return _raster->GetEnvelope();
}

const OGRSpatialReference * RasterLayer::GetSpatialReference()
{
	return _raster->GetSpatialReference();
}


bool RasterLayer::CanTransformTo(const OGRSpatialReference * spatialRef)
{
	const OGRSpatialReference * thisSpatialRef = _raster->GetSpatialReference();
	if (thisSpatialRef == spatialRef
		|| (spatialRef != nullptr && thisSpatialRef != nullptr && thisSpatialRef->IsSame(spatialRef)))
	{
		return true;
	}
	return false;
}

IDataset * RasterLayer::GetDataset()
{
	return _raster;
}

void RasterLayer::SetDataset(MyGDALRasterDataset * raster)
{
	if (!raster->IsNorthUp())
	{
		throw std::exception("不支持非北方朝上的影响！");
	}
	_raster = raster;
	_name = raster->GetName();
}

void RasterLayer::InitialMinMax(GDALRasterBand * band, int dataType, double * min, double * max, double * range)
{
	if ((GDALDataType)dataType == GDT_Byte)
	{
		*min = 0.0;
		*max = 255.0;
		*range = 255.0;
	}
	else
	{
		double 	pdfMin;
		double 	pdfMax;
		double 	pdfMean;
		double 	pdfStdDev;
		band->GetStatistics(TRUE, TRUE, &pdfMin, &pdfMax, &pdfMean, &pdfStdDev);
		double diffMin = pdfMean - pdfMin;
		double diffMax = pdfMax - pdfMin;
		double ratMinMax = diffMin / diffMax;
		if (ratMinMax > 2.0 || ratMinMax < 0.5)
		{
			*min = pdfMean - pdfStdDev;
			*max = pdfMean + pdfStdDev;
		}
		else
		{
			*min = pdfMin;
			*max = pdfMax;
		}
		*range = *max - *min;
	}
}

void RasterLayer::RestLutToLinear(unsigned char lut[256])
{
	for (int i = 0; i < 256; i++)
	{
		lut[i] = i;
	}
}

inline bool RasterLayer::IsNoDataValue(int noDataLogic, double noDataValue, double value)
{
	bool isNoData = false;
	if ((noDataLogic & EQUAL) != 0)
		isNoData = value > (noDataValue-DBL_EPSILON) && value < (noDataValue + DBL_EPSILON);
	if (isNoData == false)
	{
		if ((noDataLogic & LT) != 0)
			isNoData = value < noDataValue;
		else if ((noDataLogic & GT) != 0)
			isNoData = value > noDataValue;
	}
	return isNoData;
}

void RasterLayer::Paint(IGeoSurface * surf)
{
	if (PreparePaint(surf))
	{
		if (_outerResample)
		{
			PaintByOuterResample(surf);
		}
		else
		{
			PaintByIOResample(surf);
		}
	}
}


bool RasterLayer::PreparePaint(IGeoSurface* surf)
{
	if (_visible == false)
		return false;
	if (_alpha == 0)
		return false;
	OGREnvelope aoienvelope;
	surf->GetViewPort()->GetEnvelope(&aoienvelope);
	aoienvelope.Intersect(*(_raster->GetEnvelope()));
	if (!aoienvelope.IsInit())
		return false;

	double minInPixX = 0;
	double minInPixY = 0;
	double maxInPixX = 0;
	double maxInPixY = 0;

	_raster->Spatial2Pixel(aoienvelope.MinX, aoienvelope.MinY, &minInPixX, &minInPixY);
	_raster->Spatial2Pixel(aoienvelope.MaxX, aoienvelope.MaxY, &maxInPixX, &maxInPixY);


	_minPixX = min(minInPixX, maxInPixX);
	_minPixY = min(minInPixY, maxInPixY);
	_maxPixX = max(minInPixX, maxInPixX);
	_maxPixY = max(minInPixY, maxInPixY);

	double minInSurfX = 0;
	double minInSurfY = 0;
	double maxInSurfX = 0;
	double maxInSurfY = 0;

	surf->GetViewPort()->Spatial2Surface(aoienvelope.MinX, aoienvelope.MinY, &minInSurfX, &minInSurfY);
	surf->GetViewPort()->Spatial2Surface(aoienvelope.MaxX, aoienvelope.MaxY, &maxInSurfX, &maxInSurfY);

	_minSurfX = min(minInSurfX, maxInSurfX);
	_minSurfY = min(minInSurfY, maxInSurfY);
	_maxSurfX = max(minInSurfX, maxInSurfX);
	_maxSurfY = max(minInSurfY, maxInSurfY);

	_surfPixRatioX = (_maxPixX - _minPixX) / (_maxSurfX - _minSurfX);
	_surfPixRatioY = (_maxPixY - _minPixY) / (_maxSurfY - _minSurfY);
	_outerResample = _surfPixRatioX < 0.1 || _surfPixRatioY < 0.1;

	return true;
}


void RasterLayer::PaintByOuterResample(IGeoSurface *surf)
{
	int xRasterSize = _raster->GetGDALDataset()->GetRasterXSize();
	int yRasterSize = _raster->GetGDALDataset()->GetRasterYSize();

	int totalPaintingWidth = (int)_tgis_round(_maxSurfX - _minSurfX, 0);
	int totalPaintingHeight = (int)_tgis_round(_maxSurfY - _minSurfY, 0);

	if (totalPaintingWidth < 1 || totalPaintingHeight < 1)
		return;

	//绘制部分的范围 单位：绘制表面像素
	int paintingLeft = (int)_tgis_round(_minSurfX, 0);
	int paintingTop = (int)_tgis_round(_minSurfY, 0);
	int paintingBottom = (int)_tgis_round(_maxSurfY, 0);
	int paintingRight = (int)_tgis_round(_maxSurfX, 0);
	//绘制的部分的宽高 单位：绘制表面像素
	int paintingWidth;
	int paintingHeight;

	//读取部分的范围 单位：影像像素
	int readingLeft;
	int readingTop;
	int readingRight;
	int readingBottom;
	//读取的部分的宽高 单位：影像像素
	int readingWidth;
	int readingHeight;
	//读取的部分和绘制部分的对齐误差 单位：影像像素
	double alignRmrY;
	double alignRmrX;

	double readingTopOrg = _minPixY + (paintingTop - _minSurfY)*_surfPixRatioY;
	double readingTopFloor = floor(readingTopOrg);
	double readingTopDiff = readingTopFloor + 1 - readingTopOrg;
	if (readingTopDiff / _surfPixRatioY > 0.5)
	{
		readingTop = (int)readingTopFloor;
	}
	else
	{
		readingTop = (int)readingTopFloor + 1;
	}

	if (readingTop < 0)
	{
		readingTop = 0;
	}

	alignRmrY = readingTopOrg - readingTop;

	double readingBottomOrg = _minPixY + (paintingBottom - _minSurfY)*_surfPixRatioY;
	double readingBottomFloor = floor(readingBottomOrg);
	double readingBottomDiff = readingBottomOrg - readingBottomFloor;
	if (readingBottomDiff / _surfPixRatioY >= 0.5)
	{
		readingBottom = (int)readingBottomFloor + 2;
	}
	else
	{
		readingBottom = (int)readingBottomFloor + 1;
	}

	if (readingBottom > yRasterSize)
	{
		readingBottom = yRasterSize;
	}

	readingHeight = readingBottom - readingTop;
	paintingHeight = paintingBottom - paintingTop;

	double readingLeftOrg = _minPixX + (paintingLeft - _minSurfX)*_surfPixRatioX;
	double readingLeftFloor = floor(readingLeftOrg);
	double readingLeftDiff = readingLeftFloor + 1 - readingLeftOrg;
	if (readingLeftDiff / _surfPixRatioX > 0.5)
	{
		readingLeft = (int)readingLeftFloor;
	}
	else
	{
		readingLeft = (int)readingLeftFloor + 1;
	}

	if (readingLeft < 0)
	{
		readingLeft = 0;
	}

	alignRmrX = readingLeftOrg - readingLeft;

	double readingRightOrg = _minPixX + (paintingRight - _minSurfX)*_surfPixRatioX;
	double readingRightFloor = floor(readingRightOrg);
	double readingRightDiff = readingRightOrg - readingRightFloor;
	if (readingRightDiff / _surfPixRatioX >= 0.5)
	{
		readingRight = (int)readingRightFloor + 2;
	}
	else
	{
		readingRight = (int)readingRightFloor + 1;
	}

	if (readingRight > xRasterSize)
	{
		readingRight = xRasterSize;
	}

	readingWidth = readingRight - readingLeft;
	paintingWidth = paintingRight - paintingLeft;

	unsigned char* surfBuffer = VisualizeBufferManager::INSTANCE().AllocSurfaceBuffer();
	unsigned char* pixBuffer = VisualizeBufferManager::INSTANCE().AllocDatasetBuffer(_maxPixDataBytes);

	//RasterLayer::SetBufferAlpha(surfBuffer, paintingWidth, paintingHeight);

	(this->*OuterResample)(pixBuffer, readingLeft, alignRmrX, readingTop, alignRmrY, readingWidth, readingHeight,
		surfBuffer, paintingLeft, paintingTop, paintingWidth, paintingHeight);

	surf->DrawImage(surfBuffer, paintingLeft, paintingTop, paintingWidth, paintingHeight);

	VisualizeBufferManager::INSTANCE().FreeSurfaceBuffer(surfBuffer);
	VisualizeBufferManager::INSTANCE().FreeDatasetBuffer(pixBuffer);
}



//此时影像以原始比例或者缩小显示的
void RasterLayer::PaintByIOResample(IGeoSurface * surf)
{
	int xRasterSize = _raster->GetGDALDataset()->GetRasterXSize();
	int yRasterSize = _raster->GetGDALDataset()->GetRasterYSize();

	int totalPaintingWidth = (int)_tgis_round(_maxSurfX - _minSurfX, 0);
	int totalPaintingHeight = (int)_tgis_round(_maxSurfY - _minSurfY, 0);

	if (totalPaintingWidth < 1 || totalPaintingHeight < 1)
		return;

	//绘制部分的范围 单位：绘制表面像素
	int paintingLeft = (int)_tgis_round(_minSurfX, 0);
	int paintingTop = (int)_tgis_round(_minSurfY, 0);
	int paintingBottom = (int)_tgis_round(_maxSurfY, 0);
	int paintingRight = (int)_tgis_round(_maxSurfX, 0);
	//绘制的部分的宽高 单位：绘制表面像素
	int paintingWidth;
	int paintingHeight;

	//读取部分的范围 单位：影像像素
	int readingLeft;
	int readingTop;
	int readingRight;
	int readingBottom;
	//读取的部分的宽高 单位：影像像素
	int readingWidth;
	int readingHeight;
	//读取的部分和绘制部分的对齐误差 单位：影像像素
	double alignRmrY;
	double alignRmrX;

	double readingTopOrg = _minPixY + (paintingTop - _minSurfY)*_surfPixRatioY;
	double readingTopFloor = floor(readingTopOrg);
	double readingTopDiff = readingTopFloor + 1 - readingTopOrg;
	if (readingTopDiff / _surfPixRatioY > 0.5)
	{
		readingTop = (int)readingTopFloor;
	}
	else
	{
		readingTop = (int)readingTopFloor + 1;
	}

	if (readingTop < 0)
	{
		readingTop = 0;
	}

	alignRmrY = readingTopOrg - readingTop;

	double readingBottomOrg = _minPixY + (paintingBottom - _minSurfY)*_surfPixRatioY;
	double readingBottomFloor = floor(readingBottomOrg);
	double readingBottomDiff = readingBottomOrg - readingBottomFloor;
	if (readingBottomDiff / _surfPixRatioY >= 0.5)
	{
		readingBottom = (int)readingBottomFloor + 2;
	}
	else
	{
		readingBottom = (int)readingBottomFloor + 1;
	}

	if (readingBottom > yRasterSize)
	{
		readingBottom = yRasterSize;
	}

	readingHeight = readingBottom - readingTop;
	paintingHeight = paintingBottom - paintingTop;

	double readingLeftOrg = _minPixX + (paintingLeft - _minSurfX)*_surfPixRatioX;
	double readingLeftFloor = floor(readingLeftOrg);
	double readingLeftDiff = readingLeftFloor + 1 - readingLeftOrg;
	if (readingLeftDiff / _surfPixRatioX > 0.5)
	{
		readingLeft = (int)readingLeftFloor;
	}
	else
	{
		readingLeft = (int)readingLeftFloor + 1;
	}

	if (readingLeft < 0)
	{
		readingLeft = 0;
	}

	alignRmrX = readingLeftOrg - readingLeft;

	double readingRightOrg = _minPixX + (paintingRight - _minSurfX)*_surfPixRatioX;
	double readingRightFloor = floor(readingRightOrg);
	double readingRightDiff = readingRightOrg - readingRightFloor;
	if (readingRightDiff / _surfPixRatioX >= 0.5)
	{
		readingRight = (int)readingRightFloor + 2;
	}
	else
	{
		readingRight = (int)readingRightFloor + 1;
	}

	if (readingRight > xRasterSize)
	{
		readingRight = xRasterSize;
	}

	readingWidth = readingRight - readingLeft;
	paintingWidth = paintingRight - paintingLeft;

	unsigned char* surfBuffer = VisualizeBufferManager::INSTANCE().AllocSurfaceBuffer();
	unsigned char* pixBuffer = VisualizeBufferManager::INSTANCE().AllocDatasetBuffer(_maxPixDataBytes);

	//RasterLayer::SetBufferAlpha(surfBuffer, paintingWidth, paintingHeight);

	(this->*IOResample)(pixBuffer, readingLeft, readingTop, readingRight, readingBottom,
		surfBuffer, paintingLeft, paintingTop, paintingWidth, paintingHeight);

	surf->DrawImage(surfBuffer, paintingLeft, paintingTop, paintingWidth, paintingHeight);

	VisualizeBufferManager::INSTANCE().FreeSurfaceBuffer(surfBuffer);
	VisualizeBufferManager::INSTANCE().FreeDatasetBuffer(pixBuffer);
}


void RasterLayer::SetBufferAlpha(unsigned char * buf, int width, int height)
{
	unsigned char* surfBuf = buf +3;

	for (int i = 0; i < width*height; i++)
	{
		*surfBuf = _alpha;
		surfBuf += 4;
	}
}


END_NAME_SPACE(tGis, Core)
