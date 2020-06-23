#include "RasterPalettedLayerRender.h"
#include "MyGDALRasterDataset.h"
#include "Palette.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"


BEGIN_NAME_SPACE(tGis, Core)

const char* const RasterPalettedLayerRender::_type = "D874D8B2-9935-4EE6-AD3D-9770F54888CC";

const char * RasterPalettedLayerRender::GetType()
{
	return _type;
}

const char * RasterPalettedLayerRender::S_GetType()
{
	return _type;
}

bool RasterPalettedLayerRender::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return RasterLayerRender::IsTypeOf(type);
}

RasterPalettedLayerRender::RasterPalettedLayerRender(ILayer * layer)
	:RasterLayerRender(layer)
{
	_noDataLogic = 0;
	_noDataValue = 0.0;
	_dataBytes = 0;
	_color = nullptr;
	RasterLayerRender::OuterResample = (RasterLayerRender::OuterResampleFunc)&RasterPalettedLayerRender::OuterResample;
	RasterLayerRender::IOResample = (RasterLayerRender::IOResampleFunc)&RasterPalettedLayerRender::IOResample;
}

RasterPalettedLayerRender::RasterPalettedLayerRender(ILayer* layer, int bandIndex)
	:RasterPalettedLayerRender(layer)
{
	SetBand(bandIndex);
}


RasterPalettedLayerRender::~RasterPalettedLayerRender()
{
	if (_color != nullptr)
		_color->Release();
}

int RasterPalettedLayerRender::GetBand()
{
	return _bandIndex;
}

void RasterPalettedLayerRender::SetBand(int bandIndex)
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
}

void RasterPalettedLayerRender::SetNoDataValue(int noDataLogic, double noDataValue)
{
	_noDataLogic = noDataLogic;
	_noDataValue = noDataValue;
}

void RasterPalettedLayerRender::GetNoDataValue(int * noDataLogic, double * noDataValue)
{
	*noDataLogic = _noDataLogic;
	*noDataValue = _noDataValue;
}

void RasterPalettedLayerRender::SetPalette(Palette * color)
{
	if (color == nullptr)
		return;

	if (_color != nullptr)
		_color->Release();
	_color = color;
	if (_color != nullptr)
		_color->Reference();
}

Palette * RasterPalettedLayerRender::GetPalette()
{
	return _color;
}

void RasterPalettedLayerRender::Paint(IGeoSurface * surf)
{
	if (_color == nullptr)
		return;

	RasterLayerRender::Paint(surf);
}

void RasterPalettedLayerRender::OuterResample(unsigned char * pixBuffer, int readingLeft, double alignRmrX, int readingTop, double alignRmrY, int readingWidth, int readingHeight, unsigned char * surfBuffer, int paintingLeft, int paintingTop, int paintingWidth, int paintingHeight)
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
			int pos = (int)pixValue;

			if (RasterLayerRender::IsNoDataValue(_noDataLogic, _noDataValue, pixValue)
				|| !_color->ExistsColor(pos))
			{
				itSurfBuf[3] = unsigned char(0);
			}
			else
			{
				_color->GetColor(pos, itSurfBuf, itSurfBuf + 1, itSurfBuf + 2);
				itSurfBuf[3] = _alpha;					
			}

			itSurfBuf += 4;
		}
	}
}

void RasterPalettedLayerRender::IOResample(unsigned char * pixBuffer, int readingLeft, int readingTop, int readingRight, int readingBottom, unsigned char * surfBuffer, int paintingLeft, int paintingTop, int paintingWidth, int paintingHeight)
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
			int pos = (int)pixValue;

			if (RasterLayerRender::IsNoDataValue(_noDataLogic, _noDataValue, pixValue)
				|| !_color->ExistsColor(pos))
			{
				itSurfBuf[3] = unsigned char(0);
			}
			else
			{
				_color->GetColor(pos, itSurfBuf, itSurfBuf + 1, itSurfBuf + 2);
				itSurfBuf[3] = _alpha;
			}

			itPixBuf += _dataBytes;
			itSurfBuf += 4;
		}
	}
}


END_NAME_SPACE(tGis, Core)

