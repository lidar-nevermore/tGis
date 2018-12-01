#include "RasterGrayScaleLayer.h"
#include "MyGDALRasterDataset.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include <algorithm>


using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

const char* const RasterGrayScaleLayer::_type = "FCD79E3D-084F-4CB6-8D84-3DB1875075EB";


RasterGrayScaleLayer::RasterGrayScaleLayer(ILayerProvider* provider)
	:RasterLayer(provider)
{
	RasterLayer::OuterResample = (RasterLayer::OuterResampleFunc)&RasterGrayScaleLayer::OuterResample;
	RasterLayer::IOResample = (RasterLayer::IOResampleFunc)&RasterGrayScaleLayer::IOResample;
	_noDataLogic = 0;
	_noDataValue = 0.0;
}

RasterGrayScaleLayer::RasterGrayScaleLayer(ILayerProvider* provider, MyGDALRasterDataset* dataset, int band)
	:RasterLayer(provider)
{
	RasterLayer::OuterResample = (RasterLayer::OuterResampleFunc)&RasterGrayScaleLayer::OuterResample;
	RasterLayer::IOResample = (RasterLayer::IOResampleFunc)&RasterGrayScaleLayer::IOResample;
	SetDataset(dataset, band);
	_noDataLogic = 0;
	_noDataValue = 0.0;
}


RasterGrayScaleLayer::~RasterGrayScaleLayer()
{
}

void RasterGrayScaleLayer::SetDataset(MyGDALRasterDataset * dataset, int bandIndex)
{
	GDALRasterBand* band = dataset->GetGDALDataset()->GetRasterBand(bandIndex);
	GDALDataType dataType = band->GetRasterDataType();
	if (dataType > 7 || dataType == 0)
	{
		throw std::exception("��֧�ָ�����δ��������ظ�ʽ");
	}
	RasterLayer::SetDataset(dataset);

	_band = band;
	_dataType = dataType;
	_bandIndex = bandIndex;
	_dataBytes = GDALGetDataTypeSizeBytes((GDALDataType)_dataType);
	_maxPixDataBytes = _dataBytes;
	RasterLayer::RestLutToLinear(_lut);
	RasterLayer::InitialMinMax(_band, _dataType, &_min, &_max, &_range);
}

void RasterGrayScaleLayer::SetMinMax(double min, double max)
{
	_min = min;
	_max = max;
	_range = _max - _min;
}

void RasterGrayScaleLayer::GetMinMax(double* min, double* max)
{
	*min = _min;
	*max = _max;
}

unsigned char * RasterGrayScaleLayer::GetLut()
{
	return _lut;
}

int RasterGrayScaleLayer::GetBand()
{
	return _bandIndex;
}

//inline bool RasterGrayScaleLayer::IsNoDataValue(double value)
//{
//	if (_noDataLogic & 0x01 != 0)
//		return abs(value - _noDataValue) < DBL_EPSILON;
//	else if (_noDataLogic & 0x02 != 0)
//		return value < _noDataValue;
//	else if (_noDataLogic & 0x04 != 0)
//		return value > _noDataValue;
//	return false;
//}

void RasterGrayScaleLayer::SetNoDataValue(int noDataLogic, double noDataValue)
{
	_noDataLogic = noDataLogic;
	_noDataValue = noDataValue;
}

void RasterGrayScaleLayer::GetNoDataValue(int * noDataLogic, double * noDataValue)
{
	*noDataLogic = _noDataLogic;
	*noDataValue = _noDataValue;
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

ILayer * RasterGrayScaleLayer::Clone(IDataset * dt)
{
	if (dt == nullptr)
	{
		RasterGrayScaleLayer* layer = new RasterGrayScaleLayer(_provider);
		memcpy(layer, this, sizeof(RasterGrayScaleLayer));
		return layer;
	}

	if (!dt->IsTypeOf(MyGDALRasterDataset::S_GetType()))
		return nullptr;

	MyGDALRasterDataset* raster = (MyGDALRasterDataset*)dt;
	int bandCount = raster->GetGDALDataset()->GetRasterCount();
	if (_bandIndex > bandCount)
		return nullptr;

	GDALRasterBand* band = raster->GetGDALDataset()->GetRasterBand(_bandIndex);

	RasterGrayScaleLayer* layer = new RasterGrayScaleLayer(_provider, raster, _bandIndex);

	return layer;
}

void RasterGrayScaleLayer::OuterResample(unsigned char * pixBuffer, int readingLeft, double alignRmrX, int readingTop, double alignRmrY, int readingWidth, int readingHeight, unsigned char * surfBuffer, int paintingLeft, int paintingTop, int paintingWidth, int paintingHeight)
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
			int lutPos = (int)(256 * (pixValue - _min) / _range);
			if (lutPos < 0) lutPos = 0;
			else if (lutPos > 255) lutPos = 255;

			itSurfBuf[0] = _lut[lutPos];
			itSurfBuf[1] = itSurfBuf[0];
			itSurfBuf[2] = itSurfBuf[0];
			itSurfBuf[3] = _alpha;
			if (RasterLayer::IsNoDataValue(_noDataLogic, _noDataValue, pixValue))
				itSurfBuf[3] = unsigned char(0);

			itSurfBuf += 4;
		}
	}
}

void RasterGrayScaleLayer::IOResample(unsigned char * pixBuffer, int readingLeft, int readingTop, int readingRight, int readingBottom, unsigned char * surfBuffer, int paintingLeft, int paintingTop, int paintingWidth, int paintingHeight)
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
			int lutPos = (int)(256 * (pixValue - _min) / _range);
			if (lutPos < 0) lutPos = 0;
			else if (lutPos > 255) lutPos = 255;

			itSurfBuf[0] = _lut[lutPos];
			itSurfBuf[1] = itSurfBuf[0];
			itSurfBuf[2] = itSurfBuf[0];
			itSurfBuf[3] = _alpha;
			if (RasterLayer::IsNoDataValue(_noDataLogic, _noDataValue, pixValue))
				itSurfBuf[3] = unsigned char(0);

			itPixBuf += _dataBytes;
			itSurfBuf += 4;
		}
	}
}


END_NAME_SPACE(tGis, Core)

