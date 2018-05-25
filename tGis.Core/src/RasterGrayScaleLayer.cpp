#include "RasterGrayScaleLayer.h"
#include "MyGDALRasterDataset.h"
#include "IGeoSurface.h"
#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include <algorithm>

#include "MemoryBufferManager.h"

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

const char* const RasterGrayScaleLayer::_type = "FCD79E3D-084F-4CB6-8D84-3DB1875075EB";


RasterGrayScaleLayer::RasterGrayScaleLayer()
{
}

RasterGrayScaleLayer::RasterGrayScaleLayer(MyGDALRasterDataset* dataset, int band)
{
	SetDataset(dataset, band);
}


RasterGrayScaleLayer::~RasterGrayScaleLayer()
{
}

void RasterGrayScaleLayer::SetDataset(MyGDALRasterDataset * dataset, int band)
{
	RasterLayer::SetDataset(dataset);
	_band = dataset->GetGDALDataset()->GetRasterBand(band);
	_bandIndex = band;
	_dataType = _band->GetRasterDataType();
	if (_dataType > 7 || _dataType == 0)
	{
		throw std::exception("不支持复数和未定义的像素格式");
	}
	_dataBytes = GDALGetDataTypeSizeBytes((GDALDataType)_dataType);
	double 	pdfMin;
	double 	pdfMax;
	double 	pdfMean;
	double 	pdfStdDev;
	_band->GetStatistics(TRUE, FALSE, &pdfMin, &pdfMax, &pdfMean, &pdfStdDev);
	double diffMin = pdfMean - pdfMin;
	double diffMax = pdfMax - pdfMin;
	double ratMinMax = diffMin / diffMax;
	if (ratMinMax > 2.0 || ratMinMax < 0.5)
	{
		_min = pdfMean - pdfStdDev;
		_max = pdfMean + pdfStdDev;
	}
	else
	{
		_min = pdfMin;
		_max = pdfMax;
	}
	_range = _max - _min;
	RestLutToLinear();
}

inline void RasterGrayScaleLayer::SetMinMax(double min, double max)
{
	_min = min;
	_max = max;
	_range = _max - _min;
}

inline void RasterGrayScaleLayer::RestLutToLinear()
{
	for (int i = 0; i < 256; i++)
	{
		_lut[i] = i;
	}
}

inline unsigned char * RasterGrayScaleLayer::GetLut()
{
	return _lut;
}

const char * RasterGrayScaleLayer::GetType()
{
	return _type;
}

const char * RasterGrayScaleLayer::GetCreationString()
{
	return nullptr;
}


void RasterGrayScaleLayer::Paint(IGeoSurface * surf)
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

void RasterGrayScaleLayer::PaintByOuterResample(IGeoSurface *surf)
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
	unsigned char* pixBuffer = MemoryBufferManager::INSTANCE().AllocRasterDatasetBuffer(_dataBytes);

	RasterLayer::SetBufferAlpha(surfBuffer, MemoryBufferManager::RasterLayerBufferWidth, MemoryBufferManager::RasterLayerBufferWidth);

	for (int i = 0; i < pixBufYCount; i++)
	{
		double readingTopOrg = _minPixY + (paintingTop-_minSurfY)*_surfPixRatio;
		double readingTopFloor = floor(readingTopOrg);
		double readingTopDiff = readingTopFloor + 1 - readingTopOrg;
		if (readingTopDiff / _surfPixRatio >= 1.0)
		{
			readingTop = (int)readingTopFloor;
		}
		else
		{
			readingTop = (int)readingTopFloor + 1;
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
		if (readingBottomDiff / _surfPixRatio >= 1.0)
		{
			readingBottom = (int)readingBottomFloor + 2;
		}
		else
		{
			readingBottom = (int)readingBottomFloor;
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
			double readingLeftOrg = _minPixX + (paintingLeft-_minSurfX)*_surfPixRatio;
			double readingLeftFloor = floor(readingLeftOrg);
			double readingLeftDiff = readingLeftFloor + 1 - readingLeftOrg;
			if (readingLeftDiff / _surfPixRatio >= 1.0)
			{
				readingLeft = (int)readingLeftFloor;
			}
			else
			{
				readingLeft = (int)readingLeftFloor + 1;
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
			if (readingRightDiff / _surfPixRatio >= 1.0)
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

			_band->RasterIO(GF_Read, readingLeft, readingTop, readingWidth, readingHeight,
				pixBuffer, readingWidth, readingHeight, (GDALDataType)_dataType, 0, 0);

			unsigned char* itSurfBuf = surfBuffer;
			for (int m = 0; m < paintingHeight; m++)
			{
				int readBufRow = (int)my_round((m + paintingTop - initialPaintingTop)*_surfPixRatio + initialAlignRmrY + initialReadingTop - readingTop, 0);
				if (readBufRow < 0)
					readBufRow = 0;
				if (readBufRow >= readingHeight)
					readBufRow = readingHeight - 1;
				for (int n = 0; n < paintingWidth; n++)
				{
					int readBufCol = (int)my_round((n + paintingLeft - initialPaintingLeft)*_surfPixRatio + initialAlignRmrX + initialReadingLeft - readingLeft, 0);
					if (readBufCol < 0)
						readBufCol = 0;
					if (readBufCol >= readingWidth)
						readBufCol = readingWidth - 1;
					int readBufPos = readBufRow*readingWidth*_dataBytes + readBufCol*_dataBytes;

					double pixValue = MyGDALGetPixelValue((GDALDataType)_dataType, pixBuffer + readBufPos);
					int lutPos = (int)(256*(pixValue - _min) / _range);
					if (lutPos < 0) lutPos = 0;
					else if (lutPos > 255) lutPos = 255;

					itSurfBuf[0] = _lut[lutPos];
					itSurfBuf[1] = itSurfBuf[0];
					itSurfBuf[2] = itSurfBuf[0];

					itSurfBuf += 4;
				}
			}

			surf->DrawImage(surfBuffer, paintingLeft, paintingTop, paintingWidth, paintingHeight);

			paintingLeft = paintingRight;
		}

		paintingTop = paintingBottom;
	}

	MemoryBufferManager::INSTANCE().FreeRasterSurfaceBuffer(surfBuffer);
	MemoryBufferManager::INSTANCE().FreeRasterDatasetBuffer(pixBuffer);
}

//此时影像以原始比例或者缩小显示的
void RasterGrayScaleLayer::PaintByIOResample(IGeoSurface * surf)
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
	int paintingBottom = paintingTop;
	int paintingRight;
	//当前正要绘制的部分的宽高 单位：绘制表面像素
	int paintingWidth;
	int paintingHeight;

	//当前正要读取部分的范围 单位：影像像素
	int readingLeft;
	int readingTop;
	int readingRight;
	int readingBottom;
	//当前正要读取的部分的宽高 单位：影像像素
	int readingWidth;
	int readingHeight;

	unsigned char* surfBuffer = MemoryBufferManager::INSTANCE().AllocRasterSurfaceBuffer();
	unsigned char* pixBuffer = MemoryBufferManager::INSTANCE().AllocRasterDatasetBuffer(_dataBytes);

	RasterLayer::SetBufferAlpha(surfBuffer, MemoryBufferManager::RasterLayerBufferWidth, MemoryBufferManager::RasterLayerBufferWidth);

	while(paintingBottom < finalPaintingBottom)
	{
		bool reCalcPaintTop = false;
		double readingTopOrg = _minPixY + (paintingTop - _minSurfY)*_surfPixRatio;
		double readingTopFloor = floor(readingTopOrg);
		double readingTopDiff = readingTopFloor + 1 - readingTopOrg;
		double readingTopDiffSurfPixCount = readingTopDiff / _surfPixRatio;
		if (readingTopDiffSurfPixCount >= 1.0)
		{
			readingTop = (int)readingTopFloor;
			reCalcPaintTop = true;
		}
		else
		{
			readingTop = (int)readingTopFloor + 1;
		}

		if (readingTop < 0) 
		{
			readingTop = 0;
			reCalcPaintTop = true;
		}

		if (reCalcPaintTop)
		{
			double paintingTopOrg = _minSurfY + (readingTop - _minPixY) / _surfPixRatio;
			paintingTop = (int)my_round(paintingTopOrg, 0);
		}

		paintingBottom = paintingTop + _bufferAreaWidth;

		bool reCalcPaintingBottom = false;
		double readingBottomOrg = _minPixY + (paintingBottom - _minSurfY)*_surfPixRatio;
		double readingBottomFloor = floor(readingBottomOrg);
		double readingBottomDiff = readingBottomOrg - readingBottomFloor;
		readingBottom = (int)readingBottomFloor;
		if (readingBottom > yRasterSize)
		{
			readingBottom = yRasterSize;
			reCalcPaintingBottom = true;
		}
		if (reCalcPaintingBottom || readingBottomDiff / _surfPixRatio >= 1.0)
		{
			double paintingBottomOrg = _minSurfY + (readingBottom - _minPixY) / _surfPixRatio;
			paintingBottom = (int)my_round(paintingBottomOrg, 0);
		}

		readingHeight = readingBottom - readingTop;
		paintingHeight = paintingBottom - paintingTop;


		paintingLeft = initialPaintingLeft;
		paintingRight = paintingLeft;

		while(paintingRight<finalPaintingRight)
		{
			bool reCalcPaintLeft = false;
			double readingLeftOrg = _minPixX + (paintingLeft - _minSurfX)*_surfPixRatio;
			double readingLeftFloor = floor(readingLeftOrg);
			double readingLeftDiff = readingLeftFloor + 1 - readingLeftOrg;
			double readingLeftDiffSurfPixCount = readingLeftDiff / _surfPixRatio;

			if (readingLeftDiffSurfPixCount >= 1.0)
			{
				readingLeft = (int)readingLeftFloor;
				reCalcPaintLeft = true;
			}
			else
			{
				readingLeft = (int)readingLeftFloor + 1;
			}

			if (readingLeft < 0)
			{
				readingLeft = 0;
				reCalcPaintLeft = true;
			}

			if (reCalcPaintLeft)
			{
				double paintingLeftOrg = _minSurfX + (readingLeft - _minPixX) / _surfPixRatio;
				paintingLeft = (int)my_round(paintingLeftOrg, 0);
			}

			paintingRight = paintingLeft + _bufferAreaWidth;

			bool reCalcPaintingRight = false;
			double readingRightOrg = _minPixX + (paintingRight - _minSurfX)*_surfPixRatio;
			double readingRightFloor = floor(readingRightOrg);
			double readingRightDiff = readingRightOrg - readingRightFloor;
			readingRight = (int)readingRightFloor;
			if (readingRight > xRasterSize)
			{
				readingRight = xRasterSize;
				reCalcPaintingRight = true;
			}
			if (reCalcPaintingRight || readingRightDiff / _surfPixRatio >= 1.0)
			{
				double paintingRightOrg = _minSurfX + (readingRight - _minPixX) / _surfPixRatio;
				paintingRight = (int)my_round(paintingRightOrg, 0);
			}

			readingWidth = readingRight - readingLeft;
			paintingWidth = paintingRight - paintingLeft;

			_band->RasterIO(GF_Read, readingLeft, readingTop, readingWidth, readingHeight,
				pixBuffer, paintingWidth, paintingHeight, (GDALDataType)_dataType, 0, 0);

			unsigned char* itPixBuf = pixBuffer;
			unsigned char* itSurfBuf = surfBuffer;
			for (int m = 0; m < paintingHeight; m++)
			{
				for (int n = 0; n < paintingWidth; n++)
				{
					double pixValue = MyGDALGetPixelValue((GDALDataType)_dataType, itPixBuf);
					int lutPos = (int)(256*(pixValue - _min) / _range);
					if (lutPos < 0) lutPos = 0;
					else if (lutPos > 255) lutPos = 255;

					itSurfBuf[0] = _lut[lutPos];
					itSurfBuf[1] = itSurfBuf[0];
					itSurfBuf[2] = itSurfBuf[0];

					itPixBuf += _dataBytes;
					itSurfBuf += 4;
				}
			}

			surf->DrawImage(surfBuffer, paintingLeft, paintingTop, paintingWidth, paintingHeight);

			paintingLeft = paintingRight;
		}

		paintingTop = paintingBottom;
	}

	MemoryBufferManager::INSTANCE().FreeRasterSurfaceBuffer(surfBuffer);
	MemoryBufferManager::INSTANCE().FreeRasterDatasetBuffer(pixBuffer);
}

END_NAME_SPACE(tGis, Core)