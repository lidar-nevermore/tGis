#include "RasterRgbLayerRender.h"
#include "MyGDALRasterDataset.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include <algorithm>
#include <cmath>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

const char* const RasterRgbLayerRender::_type = "EFEB6F2A-7DD0-401D-9762-55F3F3E095D1";

const char * RasterRgbLayerRender::GetType()
{
	return _type;
}

const char * RasterRgbLayerRender::S_GetType()
{
	return _type;
}

bool RasterRgbLayerRender::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return RasterLayerRender::IsTypeOf(type);
}

RasterRgbLayerRender::RasterRgbLayerRender(ILayer* layer, int r, int g, int b)
	:RasterLayerRender(layer)
{
	_rNoDataLogic = 0;
	_rNoDataValue = 0;
	_gNoDataLogic = 0;
	_gNoDataValue = 0;
	_bNoDataLogic = 0;
	_bNoDataValue = 0;
	RasterLayerRender::OuterResample = (RasterLayerRender::OuterResampleFunc)&RasterRgbLayerRender::OuterResample;
	RasterLayerRender::IOResample = (RasterLayerRender::IOResampleFunc)&RasterRgbLayerRender::IOResample;

	GDALRasterBand* rBand = _raster->GetGDALDataset()->GetRasterBand(r);
	GDALDataType rDataType = rBand->GetRasterDataType();
	if (rDataType > 7 || rDataType == 0)
	{
		throw std::exception("不支持复数和未定义的像素格式");
	}

	GDALRasterBand* gBand = _raster->GetGDALDataset()->GetRasterBand(g);
	GDALDataType gDataType = gBand->GetRasterDataType();
	if (gDataType > 7 || gDataType == 0)
	{
		throw std::exception("不支持复数和未定义的像素格式");
	}


	GDALRasterBand* bBand = _raster->GetGDALDataset()->GetRasterBand(b);
	GDALDataType bDataType = bBand->GetRasterDataType();
	if (bDataType > 7 || bDataType == 0)
	{
		throw std::exception("不支持复数和未定义的像素格式");
	}

	_rBand = rBand;
	_rDataType = rDataType;
	_rBandIndex = r;
	_rDataBytes = GDALGetDataTypeSizeBytes((GDALDataType)_rDataType);

	_gBand = gBand;
	_gDataType = gDataType;
	_gBandIndex = g;
	_gDataBytes = GDALGetDataTypeSizeBytes((GDALDataType)_gDataType);

	_bBand = bBand;
	_bDataType = bDataType;
	_bBandIndex = b;
	_bDataBytes = GDALGetDataTypeSizeBytes((GDALDataType)_bDataType);

	_maxPixDataBytes = max(_rDataBytes, max(_gDataBytes, _bDataBytes));

	RasterLayerRender::RestLutToLinear(_rLut);
	RasterLayerRender::RestLutToLinear(_gLut);
	RasterLayerRender::RestLutToLinear(_bLut);
	RasterLayerRender::InitialMinMax(_rBand, _rDataType, &_rMin, &_rMax, &_rRange);
	RasterLayerRender::InitialMinMax(_gBand, _gDataType, &_gMin, &_gMax, &_gRange);
	RasterLayerRender::InitialMinMax(_bBand, _bDataType, &_bMin, &_bMax, &_bRange);
}


RasterRgbLayerRender::~RasterRgbLayerRender()
{
}

void RasterRgbLayerRender::SetMinMaxR(double min, double max)
{
	_rMin = min;
	_rMax = max;
	_rRange = max - min;
}

void RasterRgbLayerRender::SetMinMaxG(double min, double max)
{
	_gMin = min;
	_gMax = max;
	_gRange = max - min;
}

void RasterRgbLayerRender::SetMinMaxB(double min, double max)
{
	_bMin = min;
	_bMax = max;
	_bRange = max - min;
}

void RasterRgbLayerRender::GetMinMaxR(double * min, double * max)
{
	*min = _rMin;
	*max = _rMax;
}

void RasterRgbLayerRender::GetMinMaxG(double * min, double * max)
{
	*min = _gMin;
	*max = _gMax;
}

void RasterRgbLayerRender::GetMinMaxB(double * min, double * max)
{
	*min = _bMin;
	*max = _bMax;
}

unsigned char * RasterRgbLayerRender::GetLutR()
{
	return _rLut;
}

inline unsigned char * RasterRgbLayerRender::GetLutG()
{
	return _gLut;
}

unsigned char * RasterRgbLayerRender::GetLutB()
{
	return _bLut;
}

int RasterRgbLayerRender::GetBandR()
{
	return _rBandIndex;
}

int RasterRgbLayerRender::GetBandG()
{
	return _gBandIndex;
}

int RasterRgbLayerRender::GetBandB()
{
	return _bBandIndex;
}


void RasterRgbLayerRender::SetNoDataValueR(int noDataLogic, double noDataValue)
{
	_rNoDataLogic = noDataLogic;
	_rNoDataValue = noDataValue;
}

void RasterRgbLayerRender::GetNoDataValueR(int * noDataLogic, double * noDataValue)
{
	*noDataLogic = _rNoDataLogic;
	*noDataValue = _rNoDataValue;
}

void RasterRgbLayerRender::SetNoDataValueG(int noDataLogic, double noDataValue)
{
	_gNoDataLogic = noDataLogic;
	_gNoDataValue = noDataValue;
}

void RasterRgbLayerRender::GetNoDataValueG(int * noDataLogic, double * noDataValue)
{
	*noDataLogic = _gNoDataLogic;
	*noDataValue = _gNoDataValue;
}

void RasterRgbLayerRender::SetNoDataValueB(int noDataLogic, double noDataValue)
{
	_bNoDataLogic = noDataLogic;
	_bNoDataValue = noDataValue;
}

void RasterRgbLayerRender::GetNoDataValueB(int * noDataLogic, double * noDataValue)
{
	*noDataLogic = _bNoDataLogic;
	*noDataValue = _bNoDataValue;
}

void RasterRgbLayerRender::OuterResample(unsigned char * pixBuffer, int readingLeft, double alignRmrX, int readingTop, double alignRmrY, int readingWidth, int readingHeight, unsigned char * surfBuffer, int paintingLeft, int paintingTop, int paintingWidth, int paintingHeight)
{
	GDALRasterIOExtraArg arg;
	INIT_RASTERIO_EXTRA_ARG(arg);
	arg.eResampleAlg = GRIORA_NearestNeighbour;
	_rBand->RasterIO(GF_Read, readingLeft, readingTop, readingWidth, readingHeight,
		pixBuffer, readingWidth, readingHeight, (GDALDataType)_rDataType, 0, 0, &arg);

	unsigned char* itSurfBuf = surfBuffer;
	for (int m = 0; m < paintingHeight; m++)
	{
		int readBufRow = (int)_tgis_round((m + 0.4999999999)*_surfPixRatioY + alignRmrY, 0);
		if (readBufRow < 0)
			readBufRow = 0;
		if (readBufRow >= readingHeight)
			readBufRow = readingHeight - 1;
		for (int n = 0; n < paintingWidth; n++)
		{
			int readBufCol = (int)_tgis_round((n + 0.4999999999)*_surfPixRatioX + alignRmrX, 0);
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
			if (_gBandIndex == _rBandIndex)
				itSurfBuf[1] = itSurfBuf[2];
			if (_bBandIndex == _rBandIndex)
				itSurfBuf[0] = itSurfBuf[2];
			itSurfBuf[3] = _alpha;

			if (RasterLayerRender::IsNoDataValue(_rNoDataLogic, _rNoDataValue, pixValue))
				itSurfBuf[3] = 0;

			itSurfBuf += 4;
		}
	}

	if (_gBandIndex != _rBandIndex)
	{
		_gBand->RasterIO(GF_Read, readingLeft, readingTop, readingWidth, readingHeight,
			pixBuffer, readingWidth, readingHeight, (GDALDataType)_gDataType, 0, 0, &arg);

		unsigned char* itSurfBuf = surfBuffer;
		for (int m = 0; m < paintingHeight; m++)
		{
			int readBufRow = (int)_tgis_round((m + 0.4999999999)*_surfPixRatioY + alignRmrY, 0);
			if (readBufRow < 0)
				readBufRow = 0;
			if (readBufRow >= readingHeight)
				readBufRow = readingHeight - 1;
			for (int n = 0; n < paintingWidth; n++)
			{
				int readBufCol = (int)_tgis_round((n + 0.4999999999)*_surfPixRatioX + alignRmrX, 0);
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
				if (_bBandIndex == _gBandIndex)
					itSurfBuf[0] = itSurfBuf[1];

				if (RasterLayerRender::IsNoDataValue(_gNoDataLogic, _gNoDataValue, pixValue))
					itSurfBuf[3] = 0;

				itSurfBuf += 4;
			}
		}
	}

	if (_bBandIndex != _rBandIndex && _bBandIndex != _gBandIndex)
	{
		_bBand->RasterIO(GF_Read, readingLeft, readingTop, readingWidth, readingHeight,
			pixBuffer, readingWidth, readingHeight, (GDALDataType)_bDataType, 0, 0, &arg);

		unsigned char* itSurfBuf = surfBuffer;
		for (int m = 0; m < paintingHeight; m++)
		{
			int readBufRow = (int)_tgis_round((m + 0.4999999999)*_surfPixRatioY + alignRmrY, 0);
			if (readBufRow < 0)
				readBufRow = 0;
			if (readBufRow >= readingHeight)
				readBufRow = readingHeight - 1;
			for (int n = 0; n < paintingWidth; n++)
			{
				int readBufCol = (int)_tgis_round((n + 0.4999999999)*_surfPixRatioX + alignRmrX, 0);
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

				if (RasterLayerRender::IsNoDataValue(_bNoDataLogic, _bNoDataValue, pixValue))
					itSurfBuf[3] = 0;

				itSurfBuf += 4;
			}
		}
	}
}

void RasterRgbLayerRender::IOResample(unsigned char * pixBuffer, int readingLeft, int readingTop, int readingRight, int readingBottom, unsigned char * surfBuffer, int paintingLeft, int paintingTop, int paintingWidth, int paintingHeight)
{
	GDALRasterIOExtraArg arg;
	INIT_RASTERIO_EXTRA_ARG(arg);
	arg.eResampleAlg = GRIORA_NearestNeighbour;
	_rBand->RasterIO(GF_Read, readingLeft, readingTop, readingRight - readingLeft, readingBottom - readingTop,
		pixBuffer, paintingWidth, paintingHeight, (GDALDataType)_rDataType, 0, 0, &arg);

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
			if (_gBandIndex == _rBandIndex)
				itSurfBuf[1] = itSurfBuf[2];
			if (_bBandIndex == _rBandIndex)
				itSurfBuf[0] = itSurfBuf[2];
			itSurfBuf[3] = _alpha;

			if (RasterLayerRender::IsNoDataValue(_rNoDataLogic, _rNoDataValue, pixValue))
				itSurfBuf[3] = 0;

			itPixBuf += _rDataBytes;
			itSurfBuf += 4;
		}
	}

	if (_gBandIndex != _rBandIndex)
	{
		_gBand->RasterIO(GF_Read, readingLeft, readingTop, readingRight - readingLeft, readingBottom - readingTop,
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
				if (_bBandIndex == _gBandIndex)
					itSurfBuf[0] = itSurfBuf[1];

				if (RasterLayerRender::IsNoDataValue(_gNoDataLogic, _gNoDataValue, pixValue))
					itSurfBuf[3] = 0;

				itPixBuf += _gDataBytes;
				itSurfBuf += 4;
			}
		}
	}

	if (_bBandIndex != _rBandIndex && _bBandIndex != _gBandIndex)
	{
		_bBand->RasterIO(GF_Read, readingLeft, readingTop, readingRight - readingLeft, readingBottom - readingTop,
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

				if (RasterLayerRender::IsNoDataValue(_bNoDataLogic, _bNoDataValue, pixValue))
					itSurfBuf[3] = 0;

				itPixBuf += _bDataBytes;
				itSurfBuf += 4;
			}
		}
	}
}


END_NAME_SPACE(tGis, Core)


