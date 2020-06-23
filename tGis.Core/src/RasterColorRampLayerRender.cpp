#include "RasterColorRampLayerRender.h"
#include "MyGDALRasterDataset.h"
#include "GradientColor.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"


BEGIN_NAME_SPACE(tGis, Core)

const char* const RasterColorRampLayerRender::_type = "4F095920-4602-4C79-8FD1-63AA78316A7E";

const char * RasterColorRampLayerRender::GetType()
{
	return _type;
}

const char * RasterColorRampLayerRender::S_GetType()
{
	return _type;
}

bool RasterColorRampLayerRender::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return RasterLayerRender::IsTypeOf(type);
}


RasterColorRampLayerRender::RasterColorRampLayerRender(ILayer * layer)
	:RasterLayerRender(layer)
{
	_noDataLogic = 0;
	_noDataValue = 0.0;
	_dataBytes = 0;
	_color = nullptr;
	for (int i = 0; i < 1000; i++)
	{
		_rLut[i] = unsigned char(255.0*i/999.0);
		_gLut[i] = unsigned char(255.0*i/999.0);
		_bLut[i] = unsigned char(255.0*i/999.0);
	}
	RasterLayerRender::OuterResample = (RasterLayerRender::OuterResampleFunc)&RasterColorRampLayerRender::OuterResample;
	RasterLayerRender::IOResample = (RasterLayerRender::IOResampleFunc)&RasterColorRampLayerRender::IOResample;
}

RasterColorRampLayerRender::RasterColorRampLayerRender(ILayer* layer, int bandIndex)
	:RasterColorRampLayerRender(layer)
{
	SetBand(bandIndex);
}


RasterColorRampLayerRender::~RasterColorRampLayerRender()
{
	if (_color != nullptr)
		_color->Release();
}

void RasterColorRampLayerRender::SetMinMax(double min, double max)
{
	_min = min;
	_max = max;
	_range = _max - _min;
}

void RasterColorRampLayerRender::GetMinMax(double* min, double* max)
{
	*min = _min;
	*max = _max;
}

int RasterColorRampLayerRender::GetBand()
{
	return _bandIndex;
}

void RasterColorRampLayerRender::SetBand(int bandIndex)
{
	GDALRasterBand* band = _raster->GetGDALDataset()->GetRasterBand(bandIndex);
	GDALDataType dataType = band->GetRasterDataType();
	if (dataType > 7 || dataType == 0)
	{
		throw std::exception("不支持复数和未定义的像素格式");
	}

	_band = band;
	_dataType = dataType;
	_bandIndex = bandIndex;
	_dataBytes = GDALGetDataTypeSizeBytes((GDALDataType)_dataType);
	_maxPixDataBytes = _dataBytes;

	RasterLayerRender::InitialMinMax(_band, _dataType, &_min, &_max, &_range);
}

void RasterColorRampLayerRender::SetNoDataValue(int noDataLogic, double noDataValue)
{
	_noDataLogic = noDataLogic;
	_noDataValue = noDataValue;
}

void RasterColorRampLayerRender::GetNoDataValue(int * noDataLogic, double * noDataValue)
{
	*noDataLogic = _noDataLogic;
	*noDataValue = _noDataValue;
}

void RasterColorRampLayerRender::SetGradientColor(GradientColor * color)
{
	if (color == nullptr)
		return;

	if (_color != nullptr)
		_color->Release();
	_color = color;
	if (_color != nullptr)
		_color->Reference();

	for (int i = 0; i < 1000; i++)
	{
		double pos = i / 999.0;
		_color->GetColor(_rLut + i, _gLut + i, _bLut + i, pos);
	}
}

GradientColor * RasterColorRampLayerRender::GetGradientColor()
{
	return _color;
}

void RasterColorRampLayerRender::Paint(IGeoSurface * surf)
{
	if (_color == nullptr)
		return;

	RasterLayerRender::Paint(surf);
}

void RasterColorRampLayerRender::OuterResample(unsigned char * pixBuffer, int readingLeft, double alignRmrX, int readingTop, double alignRmrY, int readingWidth, int readingHeight, unsigned char * surfBuffer, int paintingLeft, int paintingTop, int paintingWidth, int paintingHeight)
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
			int lutPos = (int)(1000 * (pixValue - _min) / _range);
			if (lutPos < 0) lutPos = 0;
			else if (lutPos > 1000) lutPos = 1000;

			itSurfBuf[0] = _rLut[lutPos];
			itSurfBuf[1] = _gLut[lutPos];
			itSurfBuf[2] = _bLut[lutPos];
			itSurfBuf[3] = _alpha;
			if (RasterLayerRender::IsNoDataValue(_noDataLogic, _noDataValue, pixValue))
				itSurfBuf[3] = unsigned char(0);

			itSurfBuf += 4;
		}
	}
}

void RasterColorRampLayerRender::IOResample(unsigned char * pixBuffer, int readingLeft, int readingTop, int readingRight, int readingBottom, unsigned char * surfBuffer, int paintingLeft, int paintingTop, int paintingWidth, int paintingHeight)
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
			int lutPos = (int)(1000 * (pixValue - _min) / _range);
			if (lutPos < 0) lutPos = 0;
			else if (lutPos > 1000) lutPos = 1000;

			itSurfBuf[0] = _rLut[lutPos];
			itSurfBuf[1] = _gLut[lutPos];
			itSurfBuf[2] = _bLut[lutPos];
			itSurfBuf[3] = _alpha;
			if (RasterLayerRender::IsNoDataValue(_noDataLogic, _noDataValue, pixValue))
				itSurfBuf[3] = unsigned char(0);

			itPixBuf += _dataBytes;
			itSurfBuf += 4;
		}
	}
}


END_NAME_SPACE(tGis, Core)

