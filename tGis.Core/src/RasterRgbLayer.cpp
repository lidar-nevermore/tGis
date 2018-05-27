#include "RasterRgbLayer.h"
#include "MyGDALRasterDataset.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include <algorithm>
#include <cmath>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

const char* const RasterRgbLayer::_type = "EFEB6F2A-7DD0-401D-9762-55F3F3E095D1";


RasterRgbLayer::RasterRgbLayer()
{
	RasterLayer::OuterResample = (RasterLayer::OuterResampleFunc)&RasterRgbLayer::OuterResample;
	RasterLayer::IOResample = (RasterLayer::IOResampleFunc)&RasterRgbLayer::IOResample;
}

RasterRgbLayer::RasterRgbLayer(MyGDALRasterDataset * dataset, int r, int g, int b)
{
	RasterLayer::OuterResample = (RasterLayer::OuterResampleFunc)&RasterRgbLayer::OuterResample;
	RasterLayer::IOResample = (RasterLayer::IOResampleFunc)&RasterRgbLayer::IOResample;
	SetDataset(dataset, r, g, b);
}


RasterRgbLayer::~RasterRgbLayer()
{
}

const char * RasterRgbLayer::GetType()
{
	return _type;
}

const char * RasterRgbLayer::S_GetType()
{
	return _type;
}

const char * RasterRgbLayer::GetCreationString()
{
	return nullptr;
}


void RasterRgbLayer::SetDataset(MyGDALRasterDataset * dataset, int r, int g, int b)
{
	RasterLayer::SetDataset(dataset);
	RestLutToLinear();

	_rBand = dataset->GetGDALDataset()->GetRasterBand(r);
	_rBandIndex = r;
	_rDataType = _rBand->GetRasterDataType();
	if (_rDataType > 7 || _rDataType == 0)
	{
		throw std::exception("不支持复数和未定义的像素格式");
	}
	_rDataBytes = GDALGetDataTypeSizeBytes((GDALDataType)_rDataType);


	_gBand = dataset->GetGDALDataset()->GetRasterBand(g);
	_gBandIndex = g;
	_gDataType = _gBand->GetRasterDataType();
	if (_gDataType > 7 || _gDataType == 0)
	{
		throw std::exception("不支持复数和未定义的像素格式");
	}
	_gDataBytes = GDALGetDataTypeSizeBytes((GDALDataType)_gDataType);

	_bBand = dataset->GetGDALDataset()->GetRasterBand(b);
	_bBandIndex = b;
	_bDataType = _bBand->GetRasterDataType();
	if (_bDataType > 7 || _bDataType == 0)
	{
		throw std::exception("不支持复数和未定义的像素格式");
	}
	_bDataBytes = GDALGetDataTypeSizeBytes((GDALDataType)_bDataType);

	_maxPixDataBytes = max(_rDataBytes, max(_gDataBytes, _bDataBytes));

	RasterLayer::InitialMinMax(_rBand, _rDataType, &_rMin, &_rMax, &_rRange);
	RasterLayer::InitialMinMax(_gBand, _gDataType, &_gMin, &_gMax, &_gRange);
	RasterLayer::InitialMinMax(_bBand, _bDataType, &_bMin, &_bMax, &_bRange);
}

inline void RasterRgbLayer::SetMinMaxR(double min, double max)
{
	_rMin = min;
	_rMax = max;
	_rRange = max - min;
}

inline void RasterRgbLayer::SetMinMaxG(double min, double max)
{
	_gMin = min;
	_gMax = max;
	_gRange = max - min;
}

inline void RasterRgbLayer::SetMinMaxB(double min, double max)
{
	_bMin = min;
	_bMax = max;
	_bRange = max - min;
}

inline void RasterRgbLayer::RestLutToLinear()
{
	for (int i = 0; i < 256; i++)
	{
		_rLut[i] = i;
		_gLut[i] = i;
		_bLut[i] = i;
	}
}

inline unsigned char * RasterRgbLayer::GetLutR()
{
	return _rLut;
}

inline unsigned char * RasterRgbLayer::GetLutG()
{
	return _gLut;
}

inline unsigned char * RasterRgbLayer::GetLutB()
{
	return _bLut;
}

inline int RasterRgbLayer::GetBandR()
{
	return _rBandIndex;
}

inline int RasterRgbLayer::GetBandG()
{
	return _gBandIndex;
}

inline int RasterRgbLayer::GetBandB()
{
	return _bBandIndex;
}

void RasterRgbLayer::OuterResample(unsigned char * pixBuffer, int readingLeft, int initialReadingLeft, double initialAlignRmrX, int readingTop, int initialReadingTop, double initialAlignRmrY, int readingWidth, int readingHeight, unsigned char * surfBuffer, int paintingLeft, int initialPaintingLeft, int paintingTop, int initialPaintingTop, int paintingWidth, int paintingHeight)
{
	_rBand->RasterIO(GF_Read, readingLeft, readingTop, readingWidth, readingHeight,
		pixBuffer, readingWidth, readingHeight, (GDALDataType)_rDataType, 0, 0);

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
			int readBufPos = readBufRow*readingWidth*_rDataBytes + readBufCol*_rDataBytes;

			double pixValue = MyGDALGetPixelValue((GDALDataType)_rDataType, pixBuffer + readBufPos);
			int lutPos = (int)(256 * (pixValue - _rMin) / _rRange);
			if (lutPos < 0) lutPos = 0;
			else if (lutPos > 255) lutPos = 255;

			itSurfBuf[2] = _rLut[lutPos];
			if (_gBandIndex == _gBandIndex)
				itSurfBuf[1] = itSurfBuf[2];
			if (_bBandIndex == _rBandIndex)
				itSurfBuf[0] = itSurfBuf[2];

			itSurfBuf += 4;
		}
	}

	if (_gBandIndex != _rBandIndex)
	{
		_gBand->RasterIO(GF_Read, readingLeft, readingTop, readingWidth, readingHeight,
			pixBuffer, readingWidth, readingHeight, (GDALDataType)_gDataType, 0, 0);

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
				int readBufPos = readBufRow*readingWidth*_gDataBytes + readBufCol*_gDataBytes;

				double pixValue = MyGDALGetPixelValue((GDALDataType)_gDataType, pixBuffer + readBufPos);
				int lutPos = (int)(256 * (pixValue - _gMin) / _gRange);
				if (lutPos < 0) lutPos = 0;
				else if (lutPos > 255) lutPos = 255;

				itSurfBuf[1] = _gLut[lutPos];
				if (_bBandIndex == _rBandIndex)
					itSurfBuf[0] = itSurfBuf[1];

				itSurfBuf += 4;
			}
		}
	}

	if (_bBandIndex != _rBandIndex && _bBandIndex != _gBandIndex)
	{
		_bBand->RasterIO(GF_Read, readingLeft, readingTop, readingWidth, readingHeight,
			pixBuffer, readingWidth, readingHeight, (GDALDataType)_bDataType, 0, 0);

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
				int readBufPos = readBufRow*readingWidth*_bDataBytes + readBufCol*_bDataBytes;

				double pixValue = MyGDALGetPixelValue((GDALDataType)_bDataType, pixBuffer + readBufPos);
				int lutPos = (int)(256 * (pixValue - _bMin) / _bRange);
				if (lutPos < 0) lutPos = 0;
				else if (lutPos > 255) lutPos = 255;

				itSurfBuf[0] = _bLut[lutPos];

				itSurfBuf += 4;
			}
		}
	}
}

void RasterRgbLayer::IOResample(unsigned char * pixBuffer, int readingLeft, int readingTop, int readingWidth, int readingHeight, unsigned char * surfBuffer, int paintingLeft, int paintingTop, int paintingWidth, int paintingHeight)
{
	_rBand->RasterIO(GF_Read, readingLeft, readingTop, readingWidth, readingHeight,
		pixBuffer, paintingWidth, paintingHeight, (GDALDataType)_rDataType, 0, 0);

	unsigned char* itPixBuf = pixBuffer;
	unsigned char* itSurfBuf = surfBuffer;
	for (int m = 0; m < paintingHeight; m++)
	{
		for (int n = 0; n < paintingWidth; n++)
		{
			double pixValue = MyGDALGetPixelValue((GDALDataType)_rDataType, itPixBuf);
			int lutPos = (int)(256 * (pixValue - _rMin) / _rRange);
			if (lutPos < 0) lutPos = 0;
			else if (lutPos > 255) lutPos = 255;

			itSurfBuf[2] = _rLut[lutPos];
			if (_gBandIndex == _gBandIndex)
				itSurfBuf[1] = itSurfBuf[2];
			if (_bBandIndex == _rBandIndex)
				itSurfBuf[0] = itSurfBuf[2];

			itPixBuf += _rDataBytes;
			itSurfBuf += 4;
		}
	}

	if (_gBandIndex != _rBandIndex)
	{
		_gBand->RasterIO(GF_Read, readingLeft, readingTop, readingWidth, readingHeight,
			pixBuffer, paintingWidth, paintingHeight, (GDALDataType)_gDataType, 0, 0);

		unsigned char* itPixBuf = pixBuffer;
		unsigned char* itSurfBuf = surfBuffer;
		for (int m = 0; m < paintingHeight; m++)
		{
			for (int n = 0; n < paintingWidth; n++)
			{
				double pixValue = MyGDALGetPixelValue((GDALDataType)_gDataType, itPixBuf);
				int lutPos = (int)(256 * (pixValue - _gMin) / _gRange);
				if (lutPos < 0) lutPos = 0;
				else if (lutPos > 255) lutPos = 255;

				itSurfBuf[1] = _gLut[lutPos];
				if (_bBandIndex == _rBandIndex)
					itSurfBuf[0] = itSurfBuf[1];

				itPixBuf += _gDataBytes;
				itSurfBuf += 4;
			}
		}
	}

	if (_bBandIndex != _rBandIndex && _bBandIndex != _gBandIndex)
	{
		_bBand->RasterIO(GF_Read, readingLeft, readingTop, readingWidth, readingHeight,
			pixBuffer, paintingWidth, paintingHeight, (GDALDataType)_bDataType, 0, 0);

		unsigned char* itPixBuf = pixBuffer;
		unsigned char* itSurfBuf = surfBuffer;
		for (int m = 0; m < paintingHeight; m++)
		{
			for (int n = 0; n < paintingWidth; n++)
			{
				double pixValue = MyGDALGetPixelValue((GDALDataType)_bDataType, itPixBuf);
				int lutPos = (int)(256 * (pixValue - _bMin) / _bRange);
				if (lutPos < 0) lutPos = 0;
				else if (lutPos > 255) lutPos = 255;

				itSurfBuf[0] = _bLut[lutPos];

				itPixBuf += _bDataBytes;
				itSurfBuf += 4;
			}
		}
	}
}


END_NAME_SPACE(tGis, Core)


