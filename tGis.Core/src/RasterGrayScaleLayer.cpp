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

	int paintSurfWidth = _paintSurfRight - _paintSurfLeft;
	int paintSurfHeight = _paintSurfBottom - _paintSurfTop;
	int pixBufXCount = (paintSurfWidth + _bufferAreaWidth - 1) / _bufferAreaWidth;
	int pixBufYCount = (paintSurfHeight + _bufferAreaWidth - 1) / _bufferAreaWidth;

	//已经绘制完成部分的宽高 单位：绘制表面像素
	int paintedWidth = 0;
	int paintedHeight = 0;
	//当前正要绘制的部分的宽高 单位：绘制表面像素
	int paintingWidth;
	int paintingHeight;

	//已经读取完成部分的宽高 单位：影像像素
	int readedWidth = 0;
	int readedHeight = 0;
	//当前正要读取的部分的宽高 单位：影像像素
	int readingWidth;
	int readingHeight;

	for (int i = 0; i < pixBufYCount; i++)
	{
		paintedWidth = 0;
		readedWidth = 0;

		paintingHeight = min(_bufferAreaWidth, paintSurfHeight - paintedHeight);
		readingHeight = (int)my_round((i + 1)*_bufferAreaWidth*_surfPixRatio, 0) - readedHeight;

		if (_readPixTop + readedHeight + readingHeight > yRasterSize)
		{ 
			readingHeight = yRasterSize - _readPixTop - readedHeight;
		}

		for (int j = 0; j < pixBufXCount; j++)
		{
			paintingWidth = min(_bufferAreaWidth, paintSurfWidth - paintedWidth);
			readingWidth = (int)my_round((j + 1)*_bufferAreaWidth*_surfPixRatio, 0) - readedWidth;

			if (_readPixLeft + readedWidth + readingWidth > xRasterSize)
			{
				readingWidth = xRasterSize - _readPixLeft - readedWidth;
			}

			_band->RasterIO(GF_Read, _readPixLeft + readedWidth, _readPixTop + readedHeight, readingWidth, readingHeight,
				_pixBuffer, readingWidth, readingHeight, GDT_Byte, 0, 0);

			unsigned char* surfBuf = _surfBuffer;
			for (int m = 0; m < paintingHeight; m++)
			{
				int readBufRow = (int)my_round(m*_surfPixRatio + _paintSurfTopRmr/ paintingHeight, 0);
				if (readBufRow < 0)
					readBufRow = 0;
				if (readBufRow >= readingHeight)
					readBufRow = readingHeight - 1;
				for (int n = 0; n < paintingWidth; n++)
				{
					int readBufCol = (int)my_round(n*_surfPixRatio + _paintSurfLeftRmr/ paintingWidth, 0);
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

			surf->DrawImage(_surfBuffer, _paintSurfLeft + paintedWidth, _paintSurfTop + paintedHeight, paintingWidth, paintingHeight);

			readedWidth += readingWidth;
			paintedWidth += paintingWidth;
		}

		readedHeight += readingHeight;
		paintedHeight += paintingHeight;
	}
}

//此时影像以原始比例或者缩小显示的
void RasterGrayScaleLayer::PaintByIOResample(IGeoSurface * surf)
{
	//逐块读取原始像素，创建成和绘制表面一样大小的RGBA缓冲

	int xRasterSize = _dataset->GetGDALDataset()->GetRasterXSize();
	int yRasterSize = _dataset->GetGDALDataset()->GetRasterYSize();

	int paintSurfWidth = _paintSurfRight - _paintSurfLeft;
	int paintSurfHeight = _paintSurfBottom - _paintSurfTop;
	int pixBufXCount = (paintSurfWidth + _bufferAreaWidth - 1) / _bufferAreaWidth;
	int pixBufYCount = (paintSurfHeight + _bufferAreaWidth - 1) / _bufferAreaWidth;

	//已经绘制完成部分的宽高 单位：绘制表面像素
	int paintedWidth = 0;
	int paintedHeight = 0;
	//当前正要绘制的部分的宽高 单位：绘制表面像素
	int paintingWidth;
	int paintingHeight;

	//已经读取完成部分的宽高 单位：影像像素
	int readedWidth = 0;
	int readedHeight = 0;
	//当前正要读取的部分的宽高 单位：影像像素
	int readingWidth;
	int readingHeight;

	for (int i = 0; i < pixBufYCount; i++)
	{
		paintedWidth = 0;
		readedWidth = 0;

		paintingHeight = min(_bufferAreaWidth, paintSurfHeight - paintedHeight);
		readingHeight = (int)my_round((i + 1)*_bufferAreaWidth*_surfPixRatio, 0) - readedHeight;

		if (_readPixTop + readedHeight + readingHeight > yRasterSize)
		{
			readingHeight = yRasterSize - _readPixTop - readedHeight;
		}

		for (int j = 0; j < pixBufXCount; j++)
		{
			paintingWidth = min(_bufferAreaWidth, paintSurfWidth - paintedWidth);
			readingWidth = (int)my_round((j + 1)*_bufferAreaWidth*_surfPixRatio, 0) - readedWidth;

			if (_readPixLeft + readedWidth + readingWidth > xRasterSize)
			{
				readingWidth = xRasterSize - _readPixLeft - readedWidth;
			}

			int panBandMap[3] = { _bandIndex,_bandIndex,_bandIndex };
			_dataset->GetGDALDataset()->RasterIO(GF_Read, _readPixLeft + readedWidth, _readPixTop + readedHeight, readingWidth, readingHeight,
				_surfBuffer, paintingWidth, paintingHeight, GDT_Byte,3, panBandMap, 4, paintingWidth*4, 1);

			surf->DrawImage(_surfBuffer, _paintSurfLeft + paintedWidth, _paintSurfTop + paintedHeight, paintingWidth, paintingHeight);

			readedWidth += readingWidth;
			paintedWidth += paintingWidth;
		}

		readedHeight += readingHeight;
		paintedHeight += paintingHeight;
	}
}

END_NAME_SPACE(tGis, Core)