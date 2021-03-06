#include "RasterDualRampLayerRender.h"
#include "MyGDALRasterDataset.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"


BEGIN_NAME_SPACE(tGis, Core)

const char* const RasterDualRampLayerRender::_type = "7EB66993-6103-4426-AE75-FA7BC52C402B";

const char * RasterDualRampLayerRender::GetType()
{
	return _type;
}

const char * RasterDualRampLayerRender::S_GetType()
{
	return _type;
}

bool RasterDualRampLayerRender::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return RasterLayerRender::IsTypeOf(type);
}

RasterDualRampLayerRender::RasterDualRampLayerRender(ILayer * layer)
	:RasterLayerRender(layer)
{
	_leftRChannel = true;
	_leftGChannel = true;
	_leftBChannel = true;
	_rightRChannel = true;
	_rightGChannel = true;
	_rightBChannel = true;
	_noDataLogic = 0;
	_noDataValue = 0.0;
	_dataBytes = 0;
	RasterLayerRender::OuterResample = (RasterLayerRender::OuterResampleFunc)&RasterDualRampLayerRender::OuterResample;
	RasterLayerRender::IOResample = (RasterLayerRender::IOResampleFunc)&RasterDualRampLayerRender::IOResample;
}

RasterDualRampLayerRender::RasterDualRampLayerRender(ILayer* layer, int bandIndex)
	:RasterDualRampLayerRender(layer)
{
	SetBand(bandIndex);
}


RasterDualRampLayerRender::~RasterDualRampLayerRender()
{
}

void RasterDualRampLayerRender::SetMinPivotMax(double min, double pivot, double max)
{
	_min = min;
	_max = max;
	_pivot = pivot;
	_leftRange = _pivot - _min;
	_rightRange = _max - _pivot;
}

void RasterDualRampLayerRender::GetMinPivotMax(double * min, double * pivot, double * max)
{
	*min = _min;
	*max = _max;
	*pivot = _pivot;
}

unsigned char * RasterDualRampLayerRender::GetLut()
{
	return _lut;
}

int RasterDualRampLayerRender::GetBand()
{
	return _bandIndex;
}

void RasterDualRampLayerRender::SetBand(int bandIndex)
{
	GDALRasterBand* band = _raster->GetGDALDataset()->GetRasterBand(bandIndex);
	GDALDataType dataType = band->GetRasterDataType();
	if (dataType > 7 || dataType == 0)
	{
		throw std::exception("不支持复数和未定义的像素格式");
	}
	_pivot = 0;
	_band = band;
	_dataType = dataType;
	_bandIndex = bandIndex;
	_dataBytes = GDALGetDataTypeSizeBytes((GDALDataType)_dataType);
	_maxPixDataBytes = _dataBytes;
	_leftRange = _pivot - _min;
	_rightRange = _max - _pivot;
	RasterLayerRender::RestLutToLinear(_lut);
	RasterLayerRender::InitialMinMax(_band, _dataType, &_min, &_max, &_range);
}

void RasterDualRampLayerRender::SetLeftChannel(bool r, bool g, bool b)
{
	_leftRChannel = r;
	_leftGChannel = g;
	_leftBChannel = b;
}

void RasterDualRampLayerRender::GetLeftChannel(bool * r, bool * g, bool * b)
{
	*r = _leftRChannel;
	*g = _leftGChannel;
	*b = _leftBChannel;
}

void RasterDualRampLayerRender::SetRightChannel(bool r, bool g, bool b)
{
	_rightRChannel = r;
	_rightGChannel = g;
	_rightBChannel = b;
}

void RasterDualRampLayerRender::GetRightChannel(bool * r, bool * g, bool * b)
{
	*r = _rightRChannel;
	*g = _rightGChannel;
	*b = _rightBChannel;
}

void RasterDualRampLayerRender::SetNoDataValue(int noDataLogic, double noDataValue)
{
	_noDataLogic = noDataLogic;
	_noDataValue = noDataValue;
}

void RasterDualRampLayerRender::GetNoDataValue(int * noDataLogic, double * noDataValue)
{
	*noDataLogic = _noDataLogic;
	*noDataValue = _noDataValue;
}

void RasterDualRampLayerRender::OuterResample(unsigned char * pixBuffer, int readingLeft, double alignRmrX, int readingTop, double alignRmrY, int readingWidth, int readingHeight, unsigned char * surfBuffer, int paintingLeft, int paintingTop, int paintingWidth, int paintingHeight)
{
	GDALRasterIOExtraArg arg;
	INIT_RASTERIO_EXTRA_ARG(arg);
	arg.eResampleAlg = GRIORA_NearestNeighbour;
	_band->RasterIO(GF_Read, readingLeft, readingTop, readingWidth, readingHeight,
		pixBuffer, readingWidth, readingHeight, (GDALDataType)_dataType, 0, 0, &arg);

	unsigned char* itSurfBuf = surfBuffer;
	for (int m = 0; m < paintingHeight; m++)
	{
		int readBufRow = (int)floor((m + 0.4999999999)*_surfPixRatioY + alignRmrY);
		if (readBufRow < 0)
			readBufRow = 0;
		if (readBufRow >= readingHeight)
			readBufRow = readingHeight - 1;
		for (int n = 0; n < paintingWidth; n++)
		{
			int readBufCol = (int)floor((n + 0.4999999999)*_surfPixRatioX + alignRmrX);
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
				lutPos = _rightRange == 0 ? 255 : (int)(256 * (pixValue - _pivot) / _rightRange);
			}
			else
			{
				lutPos = _leftRange == 0 ? 255 : (int)(256 * (_pivot - pixValue) / _leftRange);
			}

			if (lutPos < 0) lutPos = 0;
			else if (lutPos > 255) lutPos = 255;

			if (onLeft)
			{
				itSurfBuf[0] = _leftRChannel ? _lut[lutPos] : 0;
				itSurfBuf[1] = _leftGChannel ? _lut[lutPos] : 0;
				itSurfBuf[2] = _leftBChannel ? _lut[lutPos] : 0;
			}
			else
			{
				itSurfBuf[0] = _rightRChannel ? _lut[lutPos] : 0;
				itSurfBuf[1] = _rightGChannel ? _lut[lutPos] : 0;
				itSurfBuf[2] = _rightBChannel ? _lut[lutPos] : 0;
			}

			itSurfBuf[3] = _alpha;
			if (RasterLayerRender::IsNoDataValue(_noDataLogic, _noDataValue, pixValue))
				itSurfBuf[3] = 0;

			itSurfBuf += 4;
		}
	}
}

void RasterDualRampLayerRender::IOResample(unsigned char * pixBuffer, int readingLeft, int readingTop, int readingRight, int readingBottom, unsigned char * surfBuffer, int paintingLeft, int paintingTop, int paintingWidth, int paintingHeight)
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
				lutPos = _rightRange == 0 ? 255 : (int)(256 * (pixValue - _pivot) / _rightRange);
			}
			else
			{
				lutPos = _leftRange == 0 ? 255 : (int)(256 * (_pivot - pixValue) / _leftRange);
			}

			if (lutPos < 0) lutPos = 0;
			else if (lutPos > 255) lutPos = 255;

			if (onLeft)
			{
				itSurfBuf[0] = _leftRChannel ? _lut[lutPos] : 0;
				itSurfBuf[1] = _leftGChannel ? _lut[lutPos] : 0;
				itSurfBuf[2] = _leftBChannel ? _lut[lutPos] : 0;
			}
			else
			{
				itSurfBuf[0] = _rightRChannel ? _lut[lutPos] : 0;
				itSurfBuf[1] = _rightGChannel ? _lut[lutPos] : 0;
				itSurfBuf[2] = _rightBChannel ? _lut[lutPos] : 0;
			}

			itSurfBuf[3] = _alpha;
			if (RasterLayerRender::IsNoDataValue(_noDataLogic, _noDataValue, pixValue))
				itSurfBuf[3] = 0;

			itPixBuf += _dataBytes;
			itSurfBuf += 4;
		}
	}
}



END_NAME_SPACE(tGis, Core)
