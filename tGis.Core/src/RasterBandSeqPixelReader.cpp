
#include "RasterBandSeqPixelReader.h"
#include "MyGDALRasterDataset.h"
#include <algorithm>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)


void RasterBandSeqPixelReader::Init(GDALDataset* raster,GDALRasterBand* band,int xOffset, int yOffset, int xSize, int ySize)
{
	_raster = raster;
	_band = band;
	_xOffset = xOffset;
	_yOffset = yOffset;
	_xEnd = _xOffset+xSize-1;
	_yEnd = _yOffset+ySize-1;
	_xSize = xSize;
	_ySize = ySize;
	_band->GetBlockSize(&_xBlockSize,&_yBlockSize);
	_dataType = _band->GetRasterDataType();

	if(band == nullptr)
	{
		throw std::exception("找不到波段");
	}

	if(_dataType > 7 || _dataType == 0)
	{
		throw std::exception("不支持复数和未定义的像素格式");
	}

	_dataBytes = GDALGetDataTypeSizeBytes(_dataType);
	_blockBuffer = new unsigned char[_dataBytes*_xBlockSize*_yBlockSize]();

	if(_aoiRaster != nullptr)
	{
		_aoiBlockBuffer = new unsigned char[_xBlockSize*_yBlockSize]();
		memset(_aoiBlockBuffer,1,_xBlockSize*_yBlockSize);
	}
	
	_startXBlockOff = xOffset/_xBlockSize;
	_startYBlockOff = yOffset/_yBlockSize;
	_endXBlockOff = (_xEnd+_xBlockSize-1)/_xBlockSize;
	_endYBlockOff = (_yEnd+_yBlockSize-1)/_yBlockSize;
	int xBlockSize, yBlockSize;
	_band->GetActualBlockSize(_startXBlockOff, _startYBlockOff, &xBlockSize, &yBlockSize);
	CalcPixelTraverseParam(_startXBlockOff,_startYBlockOff,xBlockSize,yBlockSize);
}

RasterBandSeqPixelReader::RasterBandSeqPixelReader(GDALDataset* raster,int band,GDALDataset* aoiRaster,int aoiBand)
{
	new (this)RasterBandSeqPixelReader(raster,raster->GetRasterBand(band),aoiRaster,aoiBand);
}

RasterBandSeqPixelReader::RasterBandSeqPixelReader(GDALDataset* raster,GDALRasterBand* band,GDALDataset* aoiRaster, int aoiBand)
{
	int xSize = raster->GetRasterXSize();
	int ySize = raster->GetRasterYSize();
	int xOffset = 0;
	int yOffset = 0;
	int xEnd = xSize-1;
	int yEnd = ySize-1;
	_aoiRaster = nullptr;
	_aoiBand = nullptr;
	_aoiBlockBuffer = nullptr;
	_aoiNoDataValue = INT_MIN;
	_aoiNoDataValueValid = false;

	if(aoiRaster != nullptr)
	{
		_aoiRaster = aoiRaster;
		_aoiBand = aoiRaster->GetRasterBand(aoiBand);

		if(_aoiBand == nullptr)
		{
			throw std::exception("找不到指定的AOI波段");
		}

		GDALDataType aoiDataType = _aoiBand->GetRasterDataType();

		if(aoiDataType != GDT_Byte)
		{
			throw std::exception("AOI波段的像素格式不是GDT_Byte");
		}

		int hasNoDataValue = 0;
		_aoiNoDataValue = _aoiBand->GetNoDataValue(&hasNoDataValue);
		_aoiNoDataValueValid = hasNoDataValue != 0;
		if (!_aoiNoDataValueValid)
		{
			_aoiNoDataValue = INT_MIN;
		}

		MyGDALRasterDataset myAoiRaster;
		myAoiRaster.Attach(_aoiRaster);
		OGREnvelope aoienvelope = *(myAoiRaster.GetEnvelope());

	    MyGDALRasterDataset myInRaster;
		myInRaster.Attach(raster);

		if (!myInRaster.IsNorthUp())
		{
			throw std::exception("不支持非北方朝上的影响！");
		}

		aoienvelope.Intersect(*(myInRaster.GetEnvelope()));
		if(!aoienvelope.IsInit())
		{
			throw std::exception("AOI与影像无交集");
		}

		double minInPixX = 0;
		double minInPixY = 0;
		double maxInPixX = 0;
		double maxInPixY = 0;

		myInRaster.Spatial2Pixel(aoienvelope.MinX,aoienvelope.MinY,&minInPixX,&minInPixY);
		myInRaster.Spatial2Pixel(aoienvelope.MaxX,aoienvelope.MaxY,&maxInPixX,&maxInPixY);

		xOffset = my_round(min(minInPixX,maxInPixX), 0);
		yOffset = my_round(min(minInPixY,maxInPixY), 0);
		xEnd = my_round(max(minInPixX,maxInPixX), 0);
		yEnd = my_round(max(minInPixY,maxInPixY), 0);

		if(xOffset < 0)
			xOffset = 0;
		if(yOffset < 0)
			yOffset = 0;

		if(xEnd >= xSize)
			xEnd = xSize-1;
		if(yEnd >= ySize)
			yEnd = ySize-1;

		xSize = xEnd-xOffset+1;
		ySize = yEnd-yOffset+1;

		double minAoiPixX = 0;
		double minAoiPixY = 0;
		double maxAoiPixX = 0;
		double maxAoiPixY = 0;
		int inAoiXSize = _aoiRaster->GetRasterXSize();
		int inAOiYSize = _aoiRaster->GetRasterYSize();

		myAoiRaster.Spatial2Pixel(aoienvelope.MinX,aoienvelope.MinY,&minAoiPixX,&minAoiPixY);
		myAoiRaster.Spatial2Pixel(aoienvelope.MaxX,aoienvelope.MaxY,&maxAoiPixX,&maxAoiPixY);

		_xAoiOffset = my_round(min(minAoiPixX,maxAoiPixX), 0);
		_yAoiOffset = my_round(min(minAoiPixY,maxAoiPixY), 0);
		_xAoiEnd = my_round(max(minAoiPixX,maxAoiPixX), 0);
		_yAoiEnd = my_round(max(minAoiPixY,maxAoiPixY), 0);

		if(_xAoiOffset < 0)
			_xAoiOffset = 0;
		if(_yAoiOffset < 0)
			_yAoiOffset = 0;

		if(_xAoiEnd >= inAoiXSize)
			_xAoiEnd = inAoiXSize-1;
		if(_yAoiEnd >= inAOiYSize)
			_yAoiEnd = inAOiYSize-1;

		_xAoiSize = _xAoiEnd-_xAoiOffset+1;
		_yAoiSize = _yAoiEnd-_yAoiOffset+1;

		_xAoiPixScale = (double)(_xAoiEnd-_xAoiOffset+1)/(xEnd-xOffset+1);
		_yAoiPixScale = (double)(_yAoiEnd-_yAoiOffset+1)/(yEnd-yOffset+1);
	}

	Init(raster,band,xOffset, yOffset, xSize, ySize);
}

RasterBandSeqPixelReader::RasterBandSeqPixelReader(GDALDataset* raster,GDALRasterBand* band,int xOffset, int yOffset, int xSize, int ySize)
{
	_aoiRaster = nullptr;
	_aoiBlockBuffer = nullptr;
	Init(raster,band,xOffset, yOffset, xSize, ySize);
}

RasterBandSeqPixelReader::RasterBandSeqPixelReader(GDALDataset* raster,int band,int xOffset, int yOffset, int xSize, int ySize)
{
	_aoiRaster = nullptr;
	_aoiBlockBuffer = nullptr;
	Init(raster,raster->GetRasterBand(band),xOffset, yOffset, xSize, ySize);
}


RasterBandSeqPixelReader::~RasterBandSeqPixelReader(void)
{
	if(_blockBuffer != nullptr)
	{
		delete [] _blockBuffer;
	}

	if(_aoiBlockBuffer != nullptr)
	{
		delete [] _aoiBlockBuffer;
	}
}

void RasterBandSeqPixelReader::CalcPixelTraverseParam(int xBlockOff,int yBlockOff,int xBlockSize,int yBlockSize)
{
	_curBlockX = xBlockOff*_xBlockSize;
	_curBlockY = yBlockOff*_yBlockSize;

	if(_xOffset<= _curBlockX)
		_curStartXPosIB = 0;
	else
		_curStartXPosIB = _xOffset-_curBlockX;

	if(_yOffset<= _curBlockY)
		_curStartYPosIB = 0;
	else
		_curStartYPosIB = _yOffset-_curBlockY;

	if(_xEnd<(_curBlockX+xBlockSize))
		_curEndXPosIB = _xEnd-_curBlockX;
	else
		_curEndXPosIB = xBlockSize-1;

	if(_yEnd<(_curBlockY+yBlockSize))
		_curEndYPosIB = _yEnd-_curBlockY;
	else
		_curEndYPosIB = yBlockSize-1;
}

double RasterBandSeqPixelReader::GetPixelValue(void* pix)
{
	double pixValue = 0.0;
	switch(_dataType)
	{
	case GDT_Byte:
		pixValue = *((unsigned char*)pix);
		break;
	case GDT_UInt16:
		pixValue = *((unsigned short*)pix);
		break;
	case GDT_Int16:
		pixValue = *((short*)pix);
		break;
	case GDT_UInt32:
		pixValue = *((unsigned int*)pix);
		break;
	case GDT_Int32:
		pixValue = *((int*)pix);
		break;
	case GDT_Float32:
		pixValue = *((float*)pix);
		break;
	case GDT_Float64:
		pixValue = *((double*)pix);
		break;
	}

	return pixValue;
}

void RasterBandSeqPixelReader::ResetPixelReading()
{
	int xBlockSize, yBlockSize;
	_band->GetActualBlockSize(_startXBlockOff, _startYBlockOff, &xBlockSize, &yBlockSize);
	CalcPixelTraverseParam(_startXBlockOff,_startYBlockOff,xBlockSize,yBlockSize);
}

void RasterBandSeqPixelReader::ForEachPixel(FOREACHPIXEL_FUNC proc, void* user)
{
	StorageBlockBuffer buffer;
	buffer.Buffer = _blockBuffer;
	for(int yb = _startYBlockOff; yb < _endYBlockOff; yb++)	
	{
		for(int xb = _startXBlockOff; xb < _endXBlockOff; xb++)
		{
			buffer.xBlockOff = xb;
			buffer.yBlockOff = yb;

			_band->ReadBlock(xb,yb,_blockBuffer);
			_band->GetActualBlockSize(xb, yb, &(buffer.xBlockSize), &(buffer.yBlockSize));
			CalcPixelTraverseParam(xb,yb,buffer.xBlockSize,buffer.yBlockSize);

			buffer.xBlockLTC = _curBlockX;
			buffer.yBlockLTC = _curBlockY;
			buffer.xValidBlockSize = _curEndXPosIB-_curStartXPosIB+1;
			buffer.yValidBlockSize = _curEndYPosIB-_curStartYPosIB+1;

			buffer.xStartPosIB = _curStartXPosIB;
			buffer.yStartPosIB = _curStartYPosIB;
			buffer.xEndPosIB = _curEndXPosIB;
			buffer.yEndPosIB = _curEndYPosIB;

			unsigned char* curAoi = nullptr;

			if(_aoiBand != nullptr)
			{
				int curBlockStartX = _curBlockX+_curStartXPosIB;
				int curBlockStartY = _curBlockY+_curStartYPosIB;
				int curAoiBlockStartX = _xAoiOffset + my_round((curBlockStartX-_xOffset)*_xAoiPixScale,0);
				int curAoiBlockStartY = _yAoiOffset + my_round((curBlockStartY-_yOffset)*_yAoiPixScale,0);
				int curAoiBlockXSize = my_round(buffer.xValidBlockSize*_xAoiPixScale,0);
				int curAoiBlockYSize = my_round(buffer.yValidBlockSize*_yAoiPixScale,0);
				GDALRasterIOExtraArg arg;
				INIT_RASTERIO_EXTRA_ARG(arg);
				arg.eResampleAlg = GRIORA_NearestNeighbour;
			    _aoiBand->RasterIO(GF_Read,curAoiBlockStartX,curAoiBlockStartY,curAoiBlockXSize,curAoiBlockYSize,_aoiBlockBuffer,buffer.xValidBlockSize,buffer.yValidBlockSize,GDT_Byte,0,0,&arg);
				curAoi = (unsigned char*)_aoiBlockBuffer;
			}

			for(int yp = _curStartYPosIB; yp <= _curEndYPosIB; yp++)
			{
				unsigned char* curLine = _blockBuffer+ yp*buffer.xBlockSize*_dataBytes;
				for(int xp = _curStartXPosIB; xp <= _curEndXPosIB; xp++)
				{
					if(curAoi == nullptr || (*curAoi > 0 && *curAoi != _aoiNoDataValue))
					{
						unsigned char* curPix = curLine+xp*_dataBytes;
						double pix = GetPixelValue(curPix);
						proc(user,_band,pix,_curBlockX+xp,_curBlockY+yp,curPix,&buffer,xp,yp);
					}

					if(curAoi != nullptr)
						curAoi += 1;
				}
			}
		}
	}
}

void RasterBandSeqPixelReader::ResetBlockReading()
{
	ResetPixelReading();
}

void RasterBandSeqPixelReader::ForEachBlock(FOREACHBLOCK_FUNC proc, void* user)
{
	StorageBlockBuffer buffer;
	buffer.Buffer = _blockBuffer;
	for(int yb = _startYBlockOff; yb < _endYBlockOff; yb++)	
	{
		for(int xb = _startXBlockOff; xb < _endXBlockOff; xb++)
		{
			buffer.xBlockOff = xb;
			buffer.yBlockOff = yb;

			_band->ReadBlock(xb,yb,_blockBuffer);
			_band->GetActualBlockSize(xb, yb, &(buffer.xBlockSize), &(buffer.yBlockSize));
			CalcPixelTraverseParam(xb,yb,buffer.xBlockSize,buffer.yBlockSize);

			buffer.xBlockLTC = _curBlockX;
			buffer.yBlockLTC = _curBlockY;
			buffer.xValidBlockSize = _curEndXPosIB-_curStartXPosIB+1;
			buffer.yValidBlockSize = _curEndYPosIB-_curStartYPosIB+1;

			buffer.xStartPosIB = _curStartXPosIB;
			buffer.yStartPosIB = _curStartYPosIB;
			buffer.xEndPosIB = _curEndXPosIB;
			buffer.yEndPosIB = _curEndYPosIB;

			proc(user,_band,&buffer);
		}
	}
}

END_NAME_SPACE(tGis, Core)