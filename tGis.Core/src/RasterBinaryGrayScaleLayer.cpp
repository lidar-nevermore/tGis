#include "RasterBinaryGrayScaleLayer.h"
#include "MyGDALRasterDataset.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include <algorithm>


using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

const char* const RasterBinaryGrayScaleLayer::_type = "7EB66993-6103-4426-AE75-FA7BC52C402B";


RasterBinaryGrayScaleLayer::RasterBinaryGrayScaleLayer()
{
	leftRChannel = true;
	leftGChannel = true;
	leftBChannel = true;
	rightRChannel = true;
	rightGChannel = true;
	rightBChannel = true;
	RasterLayer::OuterResample = (RasterLayer::OuterResampleFunc)&RasterBinaryGrayScaleLayer::OuterResample;
	RasterLayer::IOResample = (RasterLayer::IOResampleFunc)&RasterBinaryGrayScaleLayer::IOResample;
}

RasterBinaryGrayScaleLayer::RasterBinaryGrayScaleLayer(MyGDALRasterDataset * dataset, int band)
{
	leftRChannel = true;
	leftGChannel = true;
	leftBChannel = true;
	rightRChannel = true;
	rightGChannel = true;
	rightBChannel = true;
	RasterLayer::OuterResample = (RasterLayer::OuterResampleFunc)&RasterBinaryGrayScaleLayer::OuterResample;
	RasterLayer::IOResample = (RasterLayer::IOResampleFunc)&RasterBinaryGrayScaleLayer::IOResample;
	SetDataset(dataset, band);
}


RasterBinaryGrayScaleLayer::~RasterBinaryGrayScaleLayer()
{
}

inline void RasterBinaryGrayScaleLayer::SetDataset(MyGDALRasterDataset * dataset, int bandIndex)
{
	GDALRasterBand* band = dataset->GetGDALDataset()->GetRasterBand(bandIndex);
	GDALDataType dataType = band->GetRasterDataType();
	if (dataType > 7 || dataType == 0)
	{
		throw std::exception("不支持复数和未定义的像素格式");
	}
	RasterLayer::SetDataset(dataset);

	_pivot = 0;
	_band = band;
	_dataType = dataType;
	_bandIndex = bandIndex;
	_dataBytes = GDALGetDataTypeSizeBytes((GDALDataType)_dataType);
	_maxPixDataBytes = _dataBytes;
	_leftRange = _pivot - _min;
	_rightRange = _max - _pivot;
	RasterLayer::RestLutToLinear(_lut);
	RasterLayer::InitialMinMax(_band, _dataType, &_min, &_max, &_range);
}

const char * RasterBinaryGrayScaleLayer::GetType()
{
	return _type;
}

const char * RasterBinaryGrayScaleLayer::S_GetType()
{
	return _type;
}

const char * RasterBinaryGrayScaleLayer::GetCreationString()
{
	return nullptr;
}

inline void RasterBinaryGrayScaleLayer::SetMinPivotMax(double min, double pivot, double max)
{
	_min = min;
	_max = max;
	_pivot = pivot;
	_leftRange = _pivot - _min;
	_rightRange = _max - _pivot;
}

inline void RasterBinaryGrayScaleLayer::GetMinPivotMax(double * min, double * pivot, double * max)
{
	*min = _min;
	*max = _max;
	*pivot = _pivot;
}

inline unsigned char * RasterBinaryGrayScaleLayer::GetLut()
{
	return _lut;
}

inline int RasterBinaryGrayScaleLayer::GetBand()
{
	return _bandIndex;
}

inline void RasterBinaryGrayScaleLayer::SetLeftChannel(bool r, bool g, bool b)
{
	leftRChannel = r;
	leftGChannel = g;
	leftBChannel = b;
}

inline void RasterBinaryGrayScaleLayer::GetLeftChannel(bool * r, bool * g, bool * b)
{
	*r = leftRChannel;
	*g = leftGChannel;
	*b = leftBChannel;
}

inline void RasterBinaryGrayScaleLayer::SetRightChannel(bool r, bool g, bool b)
{
	rightRChannel = r;
	rightGChannel = g;
	rightBChannel = b;
}

inline void RasterBinaryGrayScaleLayer::GetRightChannel(bool * r, bool * g, bool * b)
{
	*r = rightRChannel;
	*g = rightGChannel;
	*b = rightBChannel;
}

void RasterBinaryGrayScaleLayer::OuterResample(unsigned char * pixBuffer, int readingLeft, double alignRmrX, int readingTop, double alignRmrY, int readingWidth, int readingHeight, unsigned char * surfBuffer, int paintingLeft, int paintingTop, int paintingWidth, int paintingHeight)
{
	GDALRasterIOExtraArg arg;
	INIT_RASTERIO_EXTRA_ARG(arg);
	arg.eResampleAlg = GRIORA_NearestNeighbour;
	_band->RasterIO(GF_Read, readingLeft, readingTop, readingWidth, readingHeight,
		pixBuffer, readingWidth, readingHeight, (GDALDataType)_dataType, 0, 0, &arg);

	unsigned char* itSurfBuf = surfBuffer;
	for (int m = 0; m < paintingHeight; m++)
	{
		int readBufRow = (int)floor((m + 0.4999999999)*_surfPixRatio + alignRmrY);
		if (readBufRow < 0)
			readBufRow = 0;
		if (readBufRow >= readingHeight)
			readBufRow = readingHeight - 1;
		for (int n = 0; n < paintingWidth; n++)
		{
			int readBufCol = (int)floor((n + 0.4999999999)*_surfPixRatio + alignRmrX);
			if (readBufCol < 0)
				readBufCol = 0;
			if (readBufCol >= readingWidth)
				readBufCol = readingWidth - 1;
			int readBufPos = readBufRow*readingWidth*_dataBytes + readBufCol*_dataBytes;

			double pixValue = MyGDALGetPixelValue((GDALDataType)_dataType, pixBuffer + readBufPos);

			int lutPos = 0;
			bool onLeft = true;
			if (pixValue > _pivot)
			{
				onLeft = false;
				lutPos = (int)(256 * (pixValue - _pivot) / _rightRange);
			}
			else
			{
				lutPos = (int)(256 * (_pivot - pixValue) / _leftRange);
			}

			if (lutPos < 0) lutPos = 0;
			else if (lutPos > 255) lutPos = 255;

			if (onLeft)
			{
				itSurfBuf[0] = leftBChannel ? _lut[lutPos] : 0;
				itSurfBuf[1] = leftGChannel ? _lut[lutPos] : 0;
				itSurfBuf[2] = leftRChannel ? _lut[lutPos] : 0;
			}
			else
			{
				itSurfBuf[0] = rightBChannel ? _lut[lutPos] : 0;
				itSurfBuf[1] = rightGChannel ? _lut[lutPos] : 0;
				itSurfBuf[2] = rightRChannel ? _lut[lutPos] : 0;
			}


			itSurfBuf += 4;
		}
	}
}

void RasterBinaryGrayScaleLayer::IOResample(unsigned char * pixBuffer, int readingLeft, int readingTop, int readingRight, int readingBottom, unsigned char * surfBuffer, int paintingLeft, int paintingTop, int paintingWidth, int paintingHeight)
{
	GDALRasterIOExtraArg arg;
	INIT_RASTERIO_EXTRA_ARG(arg);
	arg.eResampleAlg = GRIORA_NearestNeighbour;
	_band->RasterIO(GF_Read, readingLeft, readingTop, readingRight - readingLeft, readingBottom - readingTop,
		pixBuffer, paintingWidth, paintingHeight, (GDALDataType)_dataType, 0, 0, &arg);

	unsigned char* itPixBuf = pixBuffer;
	unsigned char* itSurfBuf = surfBuffer;
	for (int m = 0; m < paintingHeight; m++)
	{
		for (int n = 0; n < paintingWidth; n++)
		{
			double pixValue = MyGDALGetPixelValue((GDALDataType)_dataType, itPixBuf);
			int lutPos = 0;
			bool onLeft = true;
			if (pixValue > _pivot)
			{
				onLeft = false;
				lutPos = (int)(256 * (pixValue - _pivot) / _rightRange);
			}
			else
			{
				lutPos = (int)(256 * (_pivot - pixValue) / _leftRange);
			}

			if (lutPos < 0) lutPos = 0;
			else if (lutPos > 255) lutPos = 255;

			if (onLeft)
			{
				itSurfBuf[0] = leftBChannel ? _lut[lutPos] : 0;
				itSurfBuf[1] = leftGChannel ? _lut[lutPos] : 0;
				itSurfBuf[2] = leftRChannel ? _lut[lutPos] : 0;
			}
			else
			{
				itSurfBuf[0] = rightBChannel ? _lut[lutPos] : 0;
				itSurfBuf[1] = rightGChannel ? _lut[lutPos] : 0;
				itSurfBuf[2] = rightRChannel ? _lut[lutPos] : 0;
			}

			itPixBuf += _dataBytes;
			itSurfBuf += 4;
		}
	}
}



END_NAME_SPACE(tGis, Core)
