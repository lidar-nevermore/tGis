#include "RasterLayer.h"
#include "MyGDALRasterDataset.h"
#include "IGeoSurface.h"


#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include <algorithm>
#include <cmath>

#include "MemoryBufferManager.h"

BEGIN_NAME_SPACE(tGis, Core)


RasterLayer::RasterLayer()
{
	_visible = true;
	_opacity = 1.0;
	_alpha = 255;
}

RasterLayer::RasterLayer(MyGDALRasterDataset* raster)
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

IDataset * RasterLayer::GetDataset()
{
	return _raster;
}

inline void RasterLayer::SetDataset(MyGDALRasterDataset * raster)
{
	if (!raster->IsNorthUp())
	{
		throw std::exception("不支持非北方朝上的影响！");
	}
	_raster = raster;
	_name = raster->GetName();
}

inline void RasterLayer::InitialMinMax(GDALRasterBand * band, int dataType, double * min, double * max, double * range)
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


inline bool RasterLayer::PreparePaint(IGeoSurface* surf)
{
	if (_visible == false)
		return false;
	if (_alpha == 0)
		return false;

	OGREnvelope aoienvelope = *(_raster->GetEnvelope());
	aoienvelope.Intersect(*(surf->GetEnvelope()));
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

	surf->Spatial2Surface(aoienvelope.MinX, aoienvelope.MinY, &minInSurfX, &minInSurfY);
	surf->Spatial2Surface(aoienvelope.MaxX, aoienvelope.MaxY, &maxInSurfX, &maxInSurfY);

	_minSurfX = min(minInSurfX, maxInSurfX);
	_minSurfY = min(minInSurfY, maxInSurfY);
	_maxSurfX = max(minInSurfX, maxInSurfX);
	_maxSurfY = max(minInSurfY, maxInSurfY);

	_surfPixRatio = (_maxPixX - _minPixX) / (_maxSurfX - _minSurfX);
	_outerResample = _surfPixRatio < 0.1;

	return true;
}


void RasterLayer::PaintByOuterResample(IGeoSurface *surf)
{
	//逐块读取原始像素，创建成和绘制表面一样大小的RGBA缓冲

	int _bufferAreaWidth = MemoryBufferManager::RasterLayerBufferWidth;

	int xRasterSize = _raster->GetGDALDataset()->GetRasterXSize();
	int yRasterSize = _raster->GetGDALDataset()->GetRasterYSize();

	int initialPaintingLeft = (int)my_round(_minSurfX, 0);
	int initialPaintingTop = (int)my_round(_minSurfY, 0);
	int finalPaintingRight = (int)my_round(_maxSurfX, 0);
	int finalPaintingBottom = (int)my_round(_maxSurfY, 0);

	int pixBufXCount = (finalPaintingRight - initialPaintingLeft + _bufferAreaWidth - 1) / _bufferAreaWidth;
	int pixBufYCount = (finalPaintingBottom - initialPaintingTop + _bufferAreaWidth - 1) / _bufferAreaWidth;

	if (pixBufXCount < 1 || pixBufYCount < 1)
		return;

	//当前正要绘制部分的范围 单位：绘制表面像素
	int paintingLeft = initialPaintingLeft;
	int paintingTop = initialPaintingTop;
	int paintingBottom;
	int paintingRight;
	//当前正要绘制的部分的宽高 单位：绘制表面像素
	int paintingWidth;
	int paintingHeight;

	//初始影像像素和绘制表面像素对齐残差  单位：影像像素
	double initialAlignRmrX;
	double initialAlignRmrY;

	//初始读取位置
	int initialReadingLeft;
	int initialReadingTop;

	//当前正要读取部分的范围 单位：影像像素
	int readingLeft;
	int readingTop;
	int readingRight;
	int readingBottom;
	//当前正要读取的部分的宽高 单位：影像像素
	int readingWidth;
	int readingHeight;

	unsigned char* surfBuffer = MemoryBufferManager::INSTANCE().AllocRasterSurfaceBuffer();
	unsigned char* pixBuffer = MemoryBufferManager::INSTANCE().AllocRasterDatasetBuffer(_maxPixDataBytes);

	RasterLayer::SetBufferAlpha(surfBuffer, MemoryBufferManager::RasterLayerBufferWidth, MemoryBufferManager::RasterLayerBufferWidth);

	for (int i = 0; i < pixBufYCount; i++)
	{
		double readingTopOrg = _minPixY + (paintingTop - _minSurfY)*_surfPixRatio;
		double readingTopFloor = floor(readingTopOrg);
		double readingTopDiff = readingTopFloor + 1 - readingTopOrg;
		if (readingTopDiff / _surfPixRatio > 0.5)
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

		if (i == 0)
		{
			initialReadingTop = readingTop;
			initialAlignRmrY = readingTopOrg - readingTop;
		}

		paintingBottom = paintingTop + _bufferAreaWidth;
		if (paintingBottom > finalPaintingBottom)
		{
			paintingBottom = finalPaintingBottom;
		}

		double readingBottomOrg = _minPixY + (paintingBottom - _minSurfY)*_surfPixRatio;
		double readingBottomFloor = floor(readingBottomOrg);
		double readingBottomDiff = readingBottomOrg - readingBottomFloor;
		if (readingBottomDiff / _surfPixRatio >= 0.5)
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


		paintingLeft = initialPaintingLeft;

		for (int j = 0; j < pixBufXCount; j++)
		{
			double readingLeftOrg = _minPixX + (paintingLeft - _minSurfX)*_surfPixRatio;
			double readingLeftFloor = floor(readingLeftOrg);
			double readingLeftDiff = readingLeftFloor + 1 - readingLeftOrg;
			if (readingLeftDiff / _surfPixRatio > 0.5)
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

			if (j == 0)
			{
				initialReadingLeft = readingLeft;
				initialAlignRmrX = readingLeftOrg - readingLeft;
			}

			paintingRight = paintingLeft + _bufferAreaWidth;
			if (paintingRight > finalPaintingRight)
			{
				paintingRight = finalPaintingRight;
			}

			double readingRightOrg = _minPixX + (paintingRight - _minSurfX)*_surfPixRatio;
			double readingRightFloor = floor(readingRightOrg);
			double readingRightDiff = readingRightOrg - readingRightFloor;
			if (readingRightDiff / _surfPixRatio >= 0.5)
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

			(this->*OuterResample)(pixBuffer, readingLeft, initialReadingLeft, initialAlignRmrX, readingTop, initialReadingTop, initialAlignRmrY, readingWidth, readingHeight,
				surfBuffer, paintingLeft, initialPaintingLeft, paintingTop, initialPaintingTop, paintingWidth, paintingHeight);

			surf->DrawImage(surfBuffer, paintingLeft, paintingTop, paintingWidth, paintingHeight);

			paintingLeft = paintingRight;
		}

		paintingTop = paintingBottom;
	}

	MemoryBufferManager::INSTANCE().FreeRasterSurfaceBuffer(surfBuffer);
	MemoryBufferManager::INSTANCE().FreeRasterDatasetBuffer(pixBuffer);
}



//此时影像以原始比例或者缩小显示的
void RasterLayer::PaintByIOResample(IGeoSurface * surf)
{
	//逐块读取原始像素，创建成和绘制表面一样大小的RGBA缓冲

	int _bufferAreaWidth = MemoryBufferManager::RasterLayerBufferWidth;

	int xRasterSize = _raster->GetGDALDataset()->GetRasterXSize();
	int yRasterSize = _raster->GetGDALDataset()->GetRasterYSize();

	int initialPaintingLeft = (int)my_round(_minSurfX, 0);
	int initialPaintingTop = (int)my_round(_minSurfY, 0);
	int initialReadingLeft;
	int initialReadingTop;
	int finalReadingRight = (int)my_round(_maxPixX, 0);
	int finalReadingBottom = (int)my_round(_maxPixY, 0);

	int totalPaintingWidth = (int)my_round(_maxSurfX - _minSurfX, 0);
	int totalPaintingHeight = (int)my_round(_maxSurfY - _minSurfY, 0);

	if (totalPaintingWidth < 1 || totalPaintingHeight < 1)
		return;


	bool reCalcPaintTop = false;
	double initialReadingTopOrg = _minPixY + (initialPaintingTop - _minSurfY)*_surfPixRatio;
	double initialReadingTopFloor = floor(initialReadingTopOrg);
	double initialReadingTopDiff = initialReadingTopFloor + 1 - initialReadingTopOrg;
	double initialAlignRmrY = initialReadingTopDiff / _surfPixRatio;
	if (initialAlignRmrY > 0.5)
	{
		initialReadingTop = (int)initialReadingTopFloor;
		reCalcPaintTop = true;
	}
	else
	{
		initialReadingTop = (int)initialReadingTopFloor + 1;
	}

	if (initialReadingTop < 0)
	{
		initialReadingTop = 0;
		reCalcPaintTop = true;
	}

	if (reCalcPaintTop)
	{
		double initialPaintingTopOrg = _minSurfY - (_minPixY - initialReadingTop) / _surfPixRatio;
		initialPaintingTop = (int)my_round(initialPaintingTopOrg, 0);
		initialAlignRmrY = initialPaintingTopOrg - initialPaintingTop;
	}

	bool reCalcPaintLeft = false;
	double initialReadingLeftOrg = _minPixX + (initialPaintingLeft - _minSurfX)*_surfPixRatio;
	double initialReadingLeftFloor = floor(initialReadingLeftOrg);
	double initialReadingLeftDiff = initialReadingLeftFloor + 1 - initialReadingLeftOrg;
	double initialAlignRmrX = initialReadingLeftDiff / _surfPixRatio;
	if (initialAlignRmrX > 0.5)
	{
		initialReadingLeft = (int)initialReadingLeftFloor;
		reCalcPaintLeft = true;
	}
	else
	{
		initialReadingLeft = (int)initialReadingLeftFloor + 1;
	}

	if (initialReadingLeft < 0)
	{
		initialReadingLeft = 0;
		reCalcPaintLeft = true;
	}

	if (reCalcPaintLeft)
	{
		double initialPaintingLeftOrg = _minSurfX - (_minPixX - initialReadingLeft) / _surfPixRatio;
		initialPaintingLeft = (int)my_round(initialPaintingLeftOrg, 0);
		initialAlignRmrX = initialPaintingLeftOrg - initialPaintingLeft;
	}

	int readingPixWidth = (int)my_round(_bufferAreaWidth*_surfPixRatio, 0);
	int readingWidth = _bufferAreaWidth;
	int readingHeight = _bufferAreaWidth;
	//当前正要绘制的部分的宽高 单位：绘制表面像素
	int paintingWidth = _bufferAreaWidth;
	int paintingHeight = _bufferAreaWidth;

	//当前正要读取部分的范围 单位：影像像素
	int readingLeft = initialReadingLeft;
	int readingTop = initialReadingTop;
	int readingRight;
	int readingBottom;

	//当前正要绘制部分的范围 单位：绘制表面像素
	int paintingLeft = initialPaintingLeft;
	int paintingTop = initialPaintingTop;
	int paintingBottom;
	int paintingRight;


	unsigned char* surfBuffer = MemoryBufferManager::INSTANCE().AllocRasterSurfaceBuffer();
	unsigned char* pixBuffer = MemoryBufferManager::INSTANCE().AllocRasterDatasetBuffer(_maxPixDataBytes);

	RasterLayer::SetBufferAlpha(surfBuffer, MemoryBufferManager::RasterLayerBufferWidth, MemoryBufferManager::RasterLayerBufferWidth);

	while (readingTop < finalReadingBottom && readingTop < yRasterSize)
	{
		readingBottom = readingTop + readingPixWidth;
		if (readingBottom > yRasterSize)
		{
			readingBottom = yRasterSize;
			readingHeight = (int)my_round((readingBottom - readingTop)*_bufferAreaWidth / (double)readingPixWidth, 0);
		}
		paintingBottom = (int)my_round(initialPaintingTop + initialAlignRmrY + (readingBottom - initialReadingTop) / _surfPixRatio, 0);
		paintingHeight = paintingBottom - paintingTop;
		
		readingLeft = initialReadingLeft;
		paintingLeft = initialPaintingLeft;
		readingWidth = _bufferAreaWidth;
		paintingWidth = _bufferAreaWidth;

		while (readingLeft < finalReadingRight && readingLeft < xRasterSize)
		{
			readingRight = readingLeft + readingPixWidth;
			if (readingRight > xRasterSize)
			{
				readingRight = xRasterSize;
				readingWidth = (int)my_round((readingRight - readingLeft)*_bufferAreaWidth / (double)readingPixWidth, 0);
			}
			paintingRight = (int)my_round(initialPaintingLeft + initialAlignRmrX + (readingRight - initialReadingLeft) / _surfPixRatio, 0);
			paintingWidth = paintingRight - paintingLeft;

			(this->*IOResample)(pixBuffer, readingLeft, initialReadingLeft, initialAlignRmrX, readingTop, initialReadingTop, initialAlignRmrY, readingRight, readingBottom, readingWidth, readingHeight,
				surfBuffer, paintingLeft, initialPaintingLeft, paintingTop, initialPaintingTop, paintingWidth, paintingHeight);

			surf->DrawImage(surfBuffer, paintingLeft, paintingTop, paintingWidth, paintingHeight);


			readingLeft = readingRight;
			paintingLeft = paintingRight;
		}

		readingTop = readingBottom;
		paintingTop = paintingBottom;
	}

	MemoryBufferManager::INSTANCE().FreeRasterSurfaceBuffer(surfBuffer);
	MemoryBufferManager::INSTANCE().FreeRasterDatasetBuffer(pixBuffer);
}


inline void RasterLayer::SetBufferAlpha(unsigned char * buf, int width, int height)
{
	unsigned char* surfBuf = buf +3;

	for (int i = 0; i < width*width; i++)
	{
		*surfBuf = _alpha;
		surfBuf += 4;
	}
}


END_NAME_SPACE(tGis, Core)
