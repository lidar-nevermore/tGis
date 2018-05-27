#include "RasterGrayScaleLayer.h"
#include "MyGDALRasterDataset.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include <algorithm>


using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

const char* const RasterGrayScaleLayer::_type = "FCD79E3D-084F-4CB6-8D84-3DB1875075EB";


RasterGrayScaleLayer::RasterGrayScaleLayer()
{
	RasterLayer::OuterResample = (RasterLayer::OuterResampleFunc)&RasterGrayScaleLayer::OuterResample;
	RasterLayer::IOResample = (RasterLayer::IOResampleFunc)&RasterGrayScaleLayer::IOResample;
}

RasterGrayScaleLayer::RasterGrayScaleLayer(MyGDALRasterDataset* dataset, int band)
{
	RasterLayer::OuterResample = (RasterLayer::OuterResampleFunc)&RasterGrayScaleLayer::OuterResample;
	RasterLayer::IOResample = (RasterLayer::IOResampleFunc)&RasterGrayScaleLayer::IOResample;
	SetDataset(dataset, band);
}


RasterGrayScaleLayer::~RasterGrayScaleLayer()
{
}

void RasterGrayScaleLayer::SetDataset(MyGDALRasterDataset * dataset, int band)
{
	RasterLayer::SetDataset(dataset);
	RestLutToLinear();

	_band = dataset->GetGDALDataset()->GetRasterBand(band);
	_bandIndex = band;
	_dataType = _band->GetRasterDataType();
	if (_dataType > 7 || _dataType == 0)
	{
		throw std::exception("不支持复数和未定义的像素格式");
	}
	_dataBytes = GDALGetDataTypeSizeBytes((GDALDataType)_dataType);
	_maxPixDataBytes = _dataBytes;
	RasterLayer::InitialMinMax(_band, _dataType, &_min, &_max, &_range);
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

inline int RasterGrayScaleLayer::GetBand()
{
	return _bandIndex;
}

const char * RasterGrayScaleLayer::GetType()
{
	return _type;
}

const char * RasterGrayScaleLayer::S_GetType()
{
	return _type;
}

const char * RasterGrayScaleLayer::GetCreationString()
{
	return nullptr;
}


void RasterGrayScaleLayer::OuterResample(unsigned char * pixBuffer, int readingLeft, int initialReadingLeft, double initialAlignRmrX, int readingTop, int initialReadingTop, double initialAlignRmrY, int readingWidth, int readingHeight, unsigned char * surfBuffer, int paintingLeft, int initialPaintingLeft, int paintingTop, int initialPaintingTop, int paintingWidth, int paintingHeight)
{
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
			int lutPos = (int)(256 * (pixValue - _min) / _range);
			if (lutPos < 0) lutPos = 0;
			else if (lutPos > 255) lutPos = 255;

			itSurfBuf[0] = _lut[lutPos];
			itSurfBuf[1] = itSurfBuf[0];
			itSurfBuf[2] = itSurfBuf[0];

			itSurfBuf += 4;
		}
	}
}

void RasterGrayScaleLayer::IOResample(unsigned char * pixBuffer, int readingLeft, int readingTop, int readingWidth, int readingHeight, unsigned char * surfBuffer, int paintingLeft, int paintingTop, int paintingWidth, int paintingHeight)
{
	_band->RasterIO(GF_Read, readingLeft, readingTop, readingWidth, readingHeight,
		pixBuffer, paintingWidth, paintingHeight, (GDALDataType)_dataType, 0, 0);

	unsigned char* itPixBuf = pixBuffer;
	unsigned char* itSurfBuf = surfBuffer;
	for (int m = 0; m < paintingHeight; m++)
	{
		for (int n = 0; n < paintingWidth; n++)
		{
			double pixValue = MyGDALGetPixelValue((GDALDataType)_dataType, itPixBuf);
			int lutPos = (int)(256 * (pixValue - _min) / _range);
			if (lutPos < 0) lutPos = 0;
			else if (lutPos > 255) lutPos = 255;

			itSurfBuf[0] = _lut[lutPos];
			itSurfBuf[1] = itSurfBuf[0];
			itSurfBuf[2] = itSurfBuf[0];

			itPixBuf += _dataBytes;
			itSurfBuf += 4;
		}
	}
}


END_NAME_SPACE(tGis, Core)

