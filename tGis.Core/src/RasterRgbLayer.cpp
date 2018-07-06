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


RasterRgbLayer::RasterRgbLayer(ILayerProvider* provider)
	:RasterLayer(provider)
{
	_rNoDataLogic = 0;
	_rNoDataValue = 0;
	_gNoDataLogic = 0;
	_gNoDataValue = 0;
	_bNoDataLogic = 0;
	_bNoDataValue = 0;
	RasterLayer::OuterResample = (RasterLayer::OuterResampleFunc)&RasterRgbLayer::OuterResample;
	RasterLayer::IOResample = (RasterLayer::IOResampleFunc)&RasterRgbLayer::IOResample;
}

RasterRgbLayer::RasterRgbLayer(ILayerProvider* provider, MyGDALRasterDataset * dataset, int r, int g, int b)
	:RasterLayer(provider)
{
	_rNoDataLogic = 0;
	_rNoDataValue = 0;
	_gNoDataLogic = 0;
	_gNoDataValue = 0;
	_bNoDataLogic = 0;
	_bNoDataValue = 0;
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

ILayer * RasterRgbLayer::Clone(IDataset * dt)
{
	if (dt == nullptr)
	{
		RasterRgbLayer* layer = new RasterRgbLayer(_provider);
		memcpy(layer, this, sizeof(RasterRgbLayer));
		return layer;
	}

	if(!dt->IsTypeOf(MyGDALRasterDataset::S_GetType()))
		return nullptr;

	MyGDALRasterDataset* raster = (MyGDALRasterDataset*)dt;
	int bandCount = raster->GetGDALDataset()->GetRasterCount();
	if (_rBandIndex > bandCount || _gBandIndex > bandCount || _bBandIndex > bandCount)
		return nullptr;

	RasterRgbLayer* layer = new RasterRgbLayer(_provider, raster, _rBandIndex, _gBandIndex, _bBandIndex);

	return layer;
}


void RasterRgbLayer::SetDataset(MyGDALRasterDataset * dataset, int r, int g, int b)
{
	GDALRasterBand* rBand = dataset->GetGDALDataset()->GetRasterBand(r);	
	GDALDataType rDataType = rBand->GetRasterDataType();
	if (rDataType > 7 || rDataType == 0)
	{
		throw std::exception("不支持复数和未定义的像素格式");
	}

	GDALRasterBand* gBand = dataset->GetGDALDataset()->GetRasterBand(g);
	GDALDataType gDataType = gBand->GetRasterDataType();
	if (gDataType > 7 || gDataType == 0)
	{
		throw std::exception("不支持复数和未定义的像素格式");
	}
	

	GDALRasterBand* bBand = dataset->GetGDALDataset()->GetRasterBand(b);
	GDALDataType bDataType = bBand->GetRasterDataType();
	if (bDataType > 7 || bDataType == 0)
	{
		throw std::exception("不支持复数和未定义的像素格式");
	}

	RasterLayer::SetDataset(dataset);
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

	RasterLayer::RestLutToLinear(_rLut);
	RasterLayer::RestLutToLinear(_gLut);
	RasterLayer::RestLutToLinear(_bLut);
	RasterLayer::InitialMinMax(_rBand, _rDataType, &_rMin, &_rMax, &_rRange);
	RasterLayer::InitialMinMax(_gBand, _gDataType, &_gMin, &_gMax, &_gRange);
	RasterLayer::InitialMinMax(_bBand, _bDataType, &_bMin, &_bMax, &_bRange);
}

void RasterRgbLayer::SetMinMaxR(double min, double max)
{
	_rMin = min;
	_rMax = max;
	_rRange = max - min;
}

void RasterRgbLayer::SetMinMaxG(double min, double max)
{
	_gMin = min;
	_gMax = max;
	_gRange = max - min;
}

void RasterRgbLayer::SetMinMaxB(double min, double max)
{
	_bMin = min;
	_bMax = max;
	_bRange = max - min;
}

void RasterRgbLayer::GetMinMaxR(double * min, double * max)
{
	*min = _rMin;
	*max = _rMax;
}

void RasterRgbLayer::GetMinMaxG(double * min, double * max)
{
	*min = _gMin;
	*max = _gMax;
}

void RasterRgbLayer::GetMinMaxB(double * min, double * max)
{
	*min = _bMin;
	*max = _bMax;
}

unsigned char * RasterRgbLayer::GetLutR()
{
	return _rLut;
}

inline unsigned char * RasterRgbLayer::GetLutG()
{
	return _gLut;
}

unsigned char * RasterRgbLayer::GetLutB()
{
	return _bLut;
}

int RasterRgbLayer::GetBandR()
{
	return _rBandIndex;
}

int RasterRgbLayer::GetBandG()
{
	return _gBandIndex;
}

int RasterRgbLayer::GetBandB()
{
	return _bBandIndex;
}


void RasterRgbLayer::SetNoDataValueR(int noDataLogic, double noDataValue)
{
	_rNoDataLogic = noDataLogic;
	_rNoDataValue = noDataValue;
}

void RasterRgbLayer::GetNoDataValueR(int * noDataLogic, double * noDataValue)
{
	*noDataLogic = _rNoDataLogic;
	*noDataValue = _rNoDataValue;
}

void RasterRgbLayer::SetNoDataValueG(int noDataLogic, double noDataValue)
{
	_gNoDataLogic = noDataLogic;
	_gNoDataValue = noDataValue;
}

void RasterRgbLayer::GetNoDataValueG(int * noDataLogic, double * noDataValue)
{
	*noDataLogic = _gNoDataLogic;
	*noDataValue = _gNoDataValue;
}

void RasterRgbLayer::SetNoDataValueB(int noDataLogic, double noDataValue)
{
	_bNoDataLogic = noDataLogic;
	_bNoDataValue = noDataValue;
}

void RasterRgbLayer::GetNoDataValueB(int * noDataLogic, double * noDataValue)
{
	*noDataLogic = _bNoDataLogic;
	*noDataValue = _bNoDataValue;
}

void RasterRgbLayer::OuterResample(unsigned char * pixBuffer, int readingLeft, double alignRmrX, int readingTop, double alignRmrY, int readingWidth, int readingHeight, unsigned char * surfBuffer, int paintingLeft, int paintingTop, int paintingWidth, int paintingHeight)
{
	GDALRasterIOExtraArg arg;
	INIT_RASTERIO_EXTRA_ARG(arg);
	arg.eResampleAlg = GRIORA_NearestNeighbour;
	_rBand->RasterIO(GF_Read, readingLeft, readingTop, readingWidth, readingHeight,
		pixBuffer, readingWidth, readingHeight, (GDALDataType)_rDataType, 0, 0, &arg);

	unsigned char* itSurfBuf = surfBuffer;
	for (int m = 0; m < paintingHeight; m++)
	{
		int readBufRow = (int)_tgis_round((m + 0.4999999999)*_surfPixRatio + alignRmrY, 0);
		if (readBufRow < 0)
			readBufRow = 0;
		if (readBufRow >= readingHeight)
			readBufRow = readingHeight - 1;
		for (int n = 0; n < paintingWidth; n++)
		{
			int readBufCol = (int)_tgis_round((n + 0.4999999999)*_surfPixRatio + alignRmrX, 0);
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

			if (RasterLayer::IsNoDataValue(_rNoDataLogic, _rNoDataValue, pixValue))
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
			int readBufRow = (int)_tgis_round((m + 0.4999999999)*_surfPixRatio + alignRmrY, 0);
			if (readBufRow < 0)
				readBufRow = 0;
			if (readBufRow >= readingHeight)
				readBufRow = readingHeight - 1;
			for (int n = 0; n < paintingWidth; n++)
			{
				int readBufCol = (int)_tgis_round((n + 0.4999999999)*_surfPixRatio + alignRmrX, 0);
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

				if (RasterLayer::IsNoDataValue(_gNoDataLogic, _gNoDataValue, pixValue))
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
			int readBufRow = (int)_tgis_round((m + 0.4999999999)*_surfPixRatio + alignRmrY, 0);
			if (readBufRow < 0)
				readBufRow = 0;
			if (readBufRow >= readingHeight)
				readBufRow = readingHeight - 1;
			for (int n = 0; n < paintingWidth; n++)
			{
				int readBufCol = (int)_tgis_round((n + 0.4999999999)*_surfPixRatio + alignRmrX, 0);
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

				if (RasterLayer::IsNoDataValue(_bNoDataLogic, _bNoDataValue, pixValue))
					itSurfBuf[3] = 0;

				itSurfBuf += 4;
			}
		}
	}
}

void RasterRgbLayer::IOResample(unsigned char * pixBuffer, int readingLeft, int readingTop, int readingRight, int readingBottom, unsigned char * surfBuffer, int paintingLeft, int paintingTop, int paintingWidth, int paintingHeight)
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

			if (RasterLayer::IsNoDataValue(_rNoDataLogic, _rNoDataValue, pixValue))
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

				if (itSurfBuf[3] == 0 && RasterLayer::IsNoDataValue(_gNoDataLogic, _gNoDataValue, pixValue))
					itSurfBuf[3] = 0;
				else
					itSurfBuf[3] = _alpha;

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

				if (itSurfBuf[3] == 0 && RasterLayer::IsNoDataValue(_bNoDataLogic, _bNoDataValue, pixValue))
					itSurfBuf[3] = 0;
				else
					itSurfBuf[3] = _alpha;

				itPixBuf += _bDataBytes;
				itSurfBuf += 4;
			}
		}
	}
}


END_NAME_SPACE(tGis, Core)


