#include "RasterGrayScaleLayer.h"
#include "MyGDALRasterDataset.h"
#include "IGeoSurface.h"
#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include <algorithm>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

const char* const RasterGrayScaleLayer::_type = "FCD79E3D-084F-4CB6-8D84-3DB1875075EB";


RasterGrayScaleLayer::RasterGrayScaleLayer()
{
	//_bufferAreaWidth*_bufferAreaWidth RGBA
	_surfBuffer = new unsigned char[_bufferAreaWidth*_bufferAreaWidth * 4];
	//_bufferAreaWidth*_bufferAreaWidth GRAY
	_pixBuffer = new unsigned char[_bufferAreaWidth*_bufferAreaWidth];

	SetOpacity(1.0);
}

RasterGrayScaleLayer::RasterGrayScaleLayer(MyGDALRasterDataset* dataset, int band)
	:RasterLayer(dataset)
{
	//_bufferAreaWidth*_bufferAreaWidth RGBA
	_surfBuffer = new unsigned char[_bufferAreaWidth*_bufferAreaWidth*4];
	//_bufferAreaWidth*_bufferAreaWidth GRAY
	_pixBuffer = new unsigned char[_bufferAreaWidth*_bufferAreaWidth];

	SetOpacity(1.0);

	_band = dataset->GetGDALDataset()->GetRasterBand(band);
	_bandIndex = band;
}


RasterGrayScaleLayer::~RasterGrayScaleLayer()
{
	delete[] _surfBuffer;
	delete[] _pixBuffer;
}

void RasterGrayScaleLayer::SetDataset(MyGDALRasterDataset * dataset, int band)
{
	SetDataset(dataset);
	_band = dataset->GetGDALDataset()->GetRasterBand(band);
	_bandIndex = band;
}

const char * RasterGrayScaleLayer::GetType()
{
	return _type;
}

void RasterGrayScaleLayer::SetOpacity(float opacity)
{
	_opacity = opacity;
	_alpha = unsigned char(255 * _opacity);

	if (_alpha > 255)
		_alpha = 255;

	unsigned char* surfBuf = _surfBuffer+3;

	for (int i = 0; i < _bufferAreaWidth*_bufferAreaWidth; i++)
	{
		*surfBuf = _alpha;
		surfBuf += 4;
	}
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

	int xRasterSize = _dataset->GetGDALDataset()->GetRasterXSize();
	int yRasterSize = _dataset->GetGDALDataset()->GetRasterYSize();
	int pixBufXCount = (_maxSurfX - _minSurfX + _bufferAreaWidth - 1) / _bufferAreaWidth;
	int pixBufYCount = (_maxSurfY - _minSurfY + _bufferAreaWidth - 1) / _bufferAreaWidth;

	//当前正要绘制部分的范围 单位：绘制表面像素
	int paintingLeft = (int)my_round(_minSurfX, 0);
	int paintingTop = (int)my_round(_minSurfY, 0);
	int paintingBottom;
	int paintingRight;
	//当前正要绘制的部分的宽高 单位：绘制表面像素
	int paintingWidth;
	int paintingHeight;
	//初始影像像素和绘制表面像素对齐残差  单位：绘制表面像素
	double _initialAlignRmrX = _minSurfX - paintingLeft;
	double _initialAlignRmrY = _minSurfY - paintingTop;

	//当前正要读取部分的范围 单位：影像像素
	int readingLeft;
	int readingTop;
	int readingRight;
	int readingBottom;
	//当前正要读取的部分的宽高 单位：影像像素
	int readingWidth;
	int readingHeight;
	//读取的每一块影像像素和绘制表面像素对齐残差  单位：影像像素
	double _readingAlignRmrX;
	double _readingAlignRmrY;


	for (int i = 0; i < pixBufYCount; i++)
	{
		double readingTopOrg = _minPixY + (paintingTop-_minSurfY)*_surfPixRatio;
		double readingTopFloor = floor(readingTopOrg);
		double readingTopDiff = readingTopFloor + 1 - readingTopOrg;
		if (readingTopDiff / _surfPixRatio >= 1.0)
		{
			readingTop = (int)readingTopFloor;
			_readingAlignRmrY = readingTopOrg - readingTopFloor;
		}
		else
		{
			readingTop = (int)readingTopFloor + 1;
			_readingAlignRmrY = readingTopDiff;
		}

		paintingBottom = paintingTop + _bufferAreaWidth;
		if (paintingBottom > _maxSurfY)
		{
			paintingBottom = (int)my_round(_maxSurfY, 0);
		}

		double readingBottomOrg = _minPixY + (paintingBottom - _minSurfY)*_surfPixRatio;
		double readingBottomFloor = floor(readingBottomOrg);
		double readingBottomDiff = readingBottomOrg - readingBottomFloor;
		if (readingBottomDiff / _surfPixRatio >= 1.0)
		{
			readingBottom = (int)readingBottomFloor + 1;
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


		paintingLeft = (int)my_round(_minSurfX, 0);

		for (int j = 0; j < pixBufXCount; j++)
		{
			double readingLeftOrg = _minPixX + (paintingLeft-_minSurfX)*_surfPixRatio;
			double readingLeftFloor = floor(readingLeftOrg);
			double readingLeftDiff = readingLeftFloor + 1 - readingLeftOrg;
			if (readingLeftDiff / _surfPixRatio >= 1.0)
			{
				readingLeft = (int)readingLeftFloor;
				_readingAlignRmrX = readingLeftOrg - readingLeftFloor;
			}
			else
			{
				readingLeft = (int)readingLeftFloor + 1;
				_readingAlignRmrX = readingLeftDiff;
			}

			paintingRight = paintingLeft + _bufferAreaWidth;
			if (paintingRight > _maxSurfX)
			{
				paintingRight = (int)my_round(_maxSurfX, 0);
			}

			double readingRightOrg = _minPixX + (paintingRight - _minSurfX)*_surfPixRatio;
			double readingRightFloor = floor(readingRightOrg);
			double readingRightDiff = readingRightOrg - readingRightFloor;
			if (readingRightDiff / _surfPixRatio >= 1.0)
			{
				readingRight = (int)readingRightFloor + 1;
			}
			else
			{
				readingRight = (int)readingRightFloor;
			}

			if (readingRight > xRasterSize)
			{
				readingRight = xRasterSize;
			}

			readingWidth = readingRight - readingLeft;
			paintingWidth = paintingRight - paintingLeft;

			_band->RasterIO(GF_Read, readingLeft, readingTop, readingWidth, readingHeight,
				_pixBuffer, readingWidth, readingHeight, GDT_Byte, 0, 0);

			unsigned char* surfBuf = _surfBuffer;
			for (int m = 0; m < paintingHeight; m++)
			{
				int readBufRow = (int)my_round(m*_surfPixRatio + _readingAlignRmrY + _initialAlignRmrY, 0);
				if (readBufRow < 0)
					readBufRow = 0;
				if (readBufRow >= readingHeight)
					readBufRow = readingHeight - 1;
				for (int n = 0; n < paintingWidth; n++)
				{
					int readBufCol = (int)my_round(n*_surfPixRatio + _readingAlignRmrX + _initialAlignRmrX, 0);
					if (readBufCol < 0)
						readBufCol = 0;
					if (readBufCol >= readingWidth)
						readBufCol = readingWidth - 1;
					int readBufPos = readBufRow*readingWidth + readBufCol;
					surfBuf[0] = _pixBuffer[readBufPos];
					surfBuf[1] = surfBuf[0];
					surfBuf[2] = surfBuf[0];

					surfBuf += 4;
				}
			}

			surf->DrawImage(_surfBuffer, paintingLeft, paintingTop, paintingWidth, paintingHeight);

			paintingLeft = paintingRight;
		}

		paintingTop = paintingBottom;
	}
}

//此时影像以原始比例或者缩小显示的
void RasterGrayScaleLayer::PaintByIOResample(IGeoSurface * surf)
{
	//逐块读取原始像素，创建成和绘制表面一样大小的RGBA缓冲

	int xRasterSize = _dataset->GetGDALDataset()->GetRasterXSize();
	int yRasterSize = _dataset->GetGDALDataset()->GetRasterYSize();
	int pixBufXCount = (_maxSurfX - _minSurfX + _bufferAreaWidth - 1) / _bufferAreaWidth;
	int pixBufYCount = (_maxSurfY - _minSurfY + _bufferAreaWidth - 1) / _bufferAreaWidth;

	//当前正要绘制部分的范围 单位：绘制表面像素
	int paintingLeft;
	int paintingTop = (int)my_round(_minSurfY, 0);
	int paintingBottom;
	int paintingRight;
	//当前正要绘制的部分的宽高 单位：绘制表面像素
	int paintingWidth;
	int paintingHeight;
	//读取的每一块影像像素和绘制表面像素对齐残差  单位：绘制表面像素
	double _paintingAlignRmrX;
	double _paintingAlignRmrY;

	//当前正要读取部分的范围 单位：影像像素
	int readingLeft;
	int readingTop;
	int readingRight;
	int readingBottom;
	//当前正要读取的部分的宽高 单位：影像像素
	int readingWidth;
	int readingHeight;



	for (int i = 0; i < pixBufYCount; i++)
	{
		double readingTopOrg = _minPixY + (paintingTop - _minSurfY)*_surfPixRatio;
		double readingTopFloor = floor(readingTopOrg);
		double readingTopDiff = readingTopFloor + 1 - readingTopOrg;
		double readingTopDiffSurfPixCount = readingTopDiff / _surfPixRatio;
		if (readingTopDiffSurfPixCount >= 1.0)
		{
			readingTop = (int)readingTopFloor;
			double paintingTopOrg = readingTop / _surfPixRatio;
			paintingTop = (int)my_round(paintingTopOrg, 0);
			_paintingAlignRmrY = paintingTopOrg - paintingTop;
		}
		else
		{
			readingTop = (int)readingTopFloor + 1;
			_paintingAlignRmrY = readingTopDiffSurfPixCount;
		}

		paintingBottom = paintingTop + _bufferAreaWidth;
		if (paintingBottom > _maxSurfY)
		{
			paintingBottom = (int)my_round(_maxSurfY, 0);
		}

		double readingBottomOrg = _minPixY + (paintingBottom - _minSurfY)*_surfPixRatio;
		double readingBottomFloor = floor(readingBottomOrg);
		double readingBottomDiff = readingBottomOrg - readingBottomFloor;
		if (readingBottomDiff / _surfPixRatio >= 1.0)
		{
			readingBottom = (int)readingBottomFloor + 1;
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


		paintingLeft = (int)my_round(_minSurfX, 0);

		for (int j = 0; j < pixBufXCount; j++)
		{
			double readingLeftOrg = _minPixX + (paintingLeft - _minSurfX)*_surfPixRatio;
			double readingLeftFloor = floor(readingLeftOrg);
			double readingLeftDiff = readingLeftFloor + 1 - readingLeftOrg;
			double readingLeftDiffSurfPixCount = readingLeftDiff / _surfPixRatio;
			if (readingLeftDiffSurfPixCount >= 1.0)
			{
				readingLeft = (int)readingLeftFloor;
				double paintingLeftOrg = readingLeft / _surfPixRatio;
				paintingLeft = (int)my_round(paintingLeftOrg, 0);
				_paintingAlignRmrX = paintingLeftOrg - paintingLeft;
			}
			else
			{
				readingLeft = (int)readingLeftFloor + 1;
				_paintingAlignRmrX = readingLeftDiffSurfPixCount;
			}

			paintingRight = paintingLeft + _bufferAreaWidth;
			if (paintingRight > _maxSurfX)
			{
				paintingRight = (int)my_round(_maxSurfX, 0);
			}

			double readingRightOrg = _minPixX + (paintingRight - _minSurfX)*_surfPixRatio;
			double readingRightFloor = floor(readingRightOrg);
			double readingRightDiff = readingRightOrg - readingRightFloor;
			if (readingRightDiff / _surfPixRatio >= 1.0)
			{
				readingRight = (int)readingRightFloor + 1;
			}
			else
			{
				readingRight = (int)readingRightFloor;
			}

			if (readingRight > xRasterSize)
			{
				readingRight = xRasterSize;
			}

			readingWidth = readingRight - readingLeft;
			paintingWidth = paintingRight - paintingLeft;

			int panBandMap[3] = { _bandIndex,_bandIndex,_bandIndex };
			_dataset->GetGDALDataset()->RasterIO(GF_Read, readingLeft, readingTop, readingWidth, readingHeight,
				_surfBuffer, paintingWidth, paintingHeight, GDT_Byte, 3, panBandMap, 4, paintingWidth * 4, 1);

			surf->DrawImage(_surfBuffer, paintingLeft, paintingTop, paintingWidth, paintingHeight);

			paintingLeft = paintingRight;
		}

		paintingTop = paintingBottom;
	}
}

END_NAME_SPACE(tGis, Core)