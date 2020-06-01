
#include "RasterBandStorageBlockWalker.h"
#include "MyGDALFileRasterDataset.h"
#include <algorithm>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)


void RasterBandStorageBlockWalker::Init(GDALDataset* raster,GDALRasterBand* band,int xOffset, int yOffset, int xSize, int ySize)
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
	_endXBlockOff = (_xEnd+_xBlockSize)/_xBlockSize;
	_endYBlockOff = (_yEnd+_yBlockSize)/_yBlockSize;
	int xBlockSize, yBlockSize;
	_band->GetActualBlockSize(_startXBlockOff, _startYBlockOff, &xBlockSize, &yBlockSize);
	CalcPixelTraverseParam(_startXBlockOff,_startYBlockOff,xBlockSize,yBlockSize);
}

RasterBandStorageBlockWalker::RasterBandStorageBlockWalker(GDALDataset* raster,int band,GDALDataset* aoiRaster,int aoiBand)
{
	new (this)RasterBandStorageBlockWalker(raster,raster->GetRasterBand(band),aoiRaster,aoiBand);
}

RasterBandStorageBlockWalker::RasterBandStorageBlockWalker(GDALDataset* raster,GDALRasterBand* band,GDALDataset* aoiRaster, int aoiBand)
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

		MyGDALFileRasterDataset myAoiRaster;
		myAoiRaster.Attach(_aoiRaster);
		OGREnvelope aoienvelope = *(myAoiRaster.GetEnvelope());

		MyGDALFileRasterDataset myInRaster;
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

		xOffset = (int)_tgis_round(min(minInPixX,maxInPixX), 0);
		yOffset = (int)_tgis_round(min(minInPixY,maxInPixY), 0);
		xEnd = (int)_tgis_round(max(minInPixX,maxInPixX), 0);
		yEnd = (int)_tgis_round(max(minInPixY,maxInPixY), 0);

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
		int inAoiYSize = _aoiRaster->GetRasterYSize();

		myAoiRaster.Spatial2Pixel(aoienvelope.MinX,aoienvelope.MinY,&minAoiPixX,&minAoiPixY);
		myAoiRaster.Spatial2Pixel(aoienvelope.MaxX,aoienvelope.MaxY,&maxAoiPixX,&maxAoiPixY);

		_xAoiOffset = (int)_tgis_round(min(minAoiPixX,maxAoiPixX), 0);
		_yAoiOffset = (int)_tgis_round(min(minAoiPixY,maxAoiPixY), 0);
		_xAoiEnd = (int)_tgis_round(max(minAoiPixX,maxAoiPixX), 0);
		_yAoiEnd = (int)_tgis_round(max(minAoiPixY,maxAoiPixY), 0);

		if(_xAoiOffset < 0)
			_xAoiOffset = 0;
		if(_yAoiOffset < 0)
			_yAoiOffset = 0;

		if(_xAoiEnd >= inAoiXSize)
			_xAoiEnd = inAoiXSize-1;
		if(_yAoiEnd >= inAoiYSize)
			_yAoiEnd = inAoiYSize -1;

		_xAoiSize = _xAoiEnd-_xAoiOffset+1;
		_yAoiSize = _yAoiEnd-_yAoiOffset+1;

		_xAoiPixScale = (double)(_xAoiEnd-_xAoiOffset+1)/(xEnd-xOffset+1);
		_yAoiPixScale = (double)(_yAoiEnd-_yAoiOffset+1)/(yEnd-yOffset+1);
	}

	Init(raster,band,xOffset, yOffset, xSize, ySize);
}

RasterBandStorageBlockWalker::RasterBandStorageBlockWalker(GDALDataset* raster,GDALRasterBand* band,int xOffset, int yOffset, int xSize, int ySize)
{
	_aoiRaster = nullptr;
	_aoiBlockBuffer = nullptr;
	Init(raster,band,xOffset, yOffset, xSize, ySize);
}

RasterBandStorageBlockWalker::RasterBandStorageBlockWalker(GDALDataset* raster,int band,int xOffset, int yOffset, int xSize, int ySize)
{
	_aoiRaster = nullptr;
	_aoiBlockBuffer = nullptr;
	Init(raster,raster->GetRasterBand(band),xOffset, yOffset, xSize, ySize);
}


RasterBandStorageBlockWalker::~RasterBandStorageBlockWalker(void)
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

void RasterBandStorageBlockWalker::CalcPixelTraverseParam(int xBlockOff,int yBlockOff,int xBlockSize,int yBlockSize)
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

void RasterBandStorageBlockWalker::ResetPixelReading()
{
	int xBlockSize, yBlockSize;
	_band->GetActualBlockSize(_startXBlockOff, _startYBlockOff, &xBlockSize, &yBlockSize);
	CalcPixelTraverseParam(_startXBlockOff,_startYBlockOff,xBlockSize,yBlockSize);
}

void RasterBandStorageBlockWalker::ForEachPixel(FOREACHPIXEL_FUNC proc, void* user)
{
	double totalBlocks = (_endYBlockOff - _startYBlockOff)*(_endXBlockOff - _startXBlockOff);
	double arrivedBlocks = 0.0;

	StorageBlockInfo blockInfo;

	for (int yb = _startYBlockOff; yb < _endYBlockOff; yb++)
	{
		for (int xb = _startXBlockOff; xb < _endXBlockOff; xb++)
		{
			arrivedBlocks += 1;
			double progress = arrivedBlocks / totalBlocks;

			blockInfo.xBlockOff = xb;
			blockInfo.yBlockOff = yb;

			_band->ReadBlock(xb, yb, _blockBuffer);

			int xBlockSize;
			int yBlockSize;
			_band->GetActualBlockSize(xb, yb, &xBlockSize, &yBlockSize);

			blockInfo.xBlockSize = xBlockSize;
			blockInfo.yBlockSize = yBlockSize;

			CalcPixelTraverseParam(xb, yb, xBlockSize, yBlockSize);

			blockInfo.xBlockLTC = _curBlockX;
			blockInfo.yBlockLTC = _curBlockY;
			blockInfo.xValidBlockSize = _curEndXPosIB - _curStartXPosIB + 1;
			blockInfo.yValidBlockSize = _curEndYPosIB - _curStartYPosIB + 1;

			blockInfo.xStartPosIB = _curStartXPosIB;
			blockInfo.yStartPosIB = _curStartYPosIB;
			blockInfo.xEndPosIB = _curEndXPosIB;
			blockInfo.yEndPosIB = _curEndYPosIB;

			unsigned char* curAoi = nullptr;

			if (_aoiBand != nullptr)
			{
				int curBlockStartX = _curBlockX + _curStartXPosIB;
				int curBlockStartY = _curBlockY + _curStartYPosIB;
				int curAoiBlockStartX = _xAoiOffset + (int)_tgis_round((curBlockStartX - _xOffset)*_xAoiPixScale, 0);
				int curAoiBlockStartY = _yAoiOffset + (int)_tgis_round((curBlockStartY - _yOffset)*_yAoiPixScale, 0);
				int curAoiBlockXSize = (int)_tgis_round(blockInfo.xValidBlockSize*_xAoiPixScale, 0);
				if (curAoiBlockXSize == 0)
					curAoiBlockXSize = 1;
				int curAoiBlockYSize = (int)_tgis_round(blockInfo.yValidBlockSize*_yAoiPixScale, 0);
				if (curAoiBlockYSize == 0)
					curAoiBlockYSize = 1;
				GDALRasterIOExtraArg arg;
				INIT_RASTERIO_EXTRA_ARG(arg);
				arg.eResampleAlg = GRIORA_NearestNeighbour;
				_aoiBand->RasterIO(GF_Read, curAoiBlockStartX, curAoiBlockStartY, curAoiBlockXSize, curAoiBlockYSize, _aoiBlockBuffer, blockInfo.xValidBlockSize, blockInfo.yValidBlockSize, GDT_Byte, 0, 0, &arg);
				curAoi = (unsigned char*)_aoiBlockBuffer;
			}

			for (int yp = _curStartYPosIB; yp <= _curEndYPosIB; yp++)
			{
				unsigned char* curLine = _blockBuffer + yp*blockInfo.xBlockSize*_dataBytes;
				for (int xp = _curStartXPosIB; xp <= _curEndXPosIB; xp++)
				{
					if (curAoi == nullptr || (*curAoi > 0 && *curAoi != _aoiNoDataValue))
					{
						unsigned char* curPix = curLine + xp*_dataBytes;
						double pix = MyGDALGetPixelValue(_dataType, curPix);
						proc(user,
							_band, 
							pix, _curBlockX + xp, _curBlockY + yp, xp, yp, curPix,
							&blockInfo, _blockBuffer,
							_dataType, _dataBytes,
							progress);
					}

					if (curAoi != nullptr)
						curAoi += 1;
				}
			}
		}
	}
}

void RasterBandStorageBlockWalker::ResetBlockReading()
{
	ResetPixelReading();
}

void RasterBandStorageBlockWalker::ForEachBlock(FOREACHBLOCK_FUNC proc, void* user)
{
	double totalBlocks = (_endYBlockOff - _startYBlockOff)*(_endXBlockOff - _startXBlockOff);
	double arrivedBlocks = 0.0;

	StorageBlockInfo blockInfo;
	for (int yb = _startYBlockOff; yb < _endYBlockOff; yb++)
	{
		for (int xb = _startXBlockOff; xb < _endXBlockOff; xb++)
		{
			arrivedBlocks += 1;
			double progress = arrivedBlocks / totalBlocks;

			blockInfo.xBlockOff = xb;
			blockInfo.yBlockOff = yb;

			_band->ReadBlock(xb, yb, _blockBuffer);

			int xBlockSize;
			int yBlockSize;
			_band->GetActualBlockSize(xb, yb, &xBlockSize, &yBlockSize);

			blockInfo.xBlockSize = xBlockSize;
			blockInfo.yBlockSize = yBlockSize;

			CalcPixelTraverseParam(xb, yb, xBlockSize, yBlockSize);

			blockInfo.xBlockLTC = _curBlockX;
			blockInfo.yBlockLTC = _curBlockY;
			blockInfo.xValidBlockSize = _curEndXPosIB - _curStartXPosIB + 1;
			blockInfo.yValidBlockSize = _curEndYPosIB - _curStartYPosIB + 1;

			blockInfo.xStartPosIB = _curStartXPosIB;
			blockInfo.yStartPosIB = _curStartYPosIB;
			blockInfo.xEndPosIB = _curEndXPosIB;
			blockInfo.yEndPosIB = _curEndYPosIB;

			if (_aoiBand != nullptr)
			{
				int curBlockStartX = _curBlockX + _curStartXPosIB;
				int curBlockStartY = _curBlockY + _curStartYPosIB;
				int curAoiBlockStartX = _xAoiOffset + (int)_tgis_round((curBlockStartX - _xOffset)*_xAoiPixScale, 0);
				int curAoiBlockStartY = _yAoiOffset + (int)_tgis_round((curBlockStartY - _yOffset)*_yAoiPixScale, 0);
				int curAoiBlockXSize = (int)_tgis_round(blockInfo.xValidBlockSize*_xAoiPixScale, 0);
				if (curAoiBlockXSize == 0)
					curAoiBlockXSize = 1;
				int curAoiBlockYSize = (int)_tgis_round(blockInfo.yValidBlockSize*_yAoiPixScale, 0);
				if (curAoiBlockYSize == 0)
					curAoiBlockYSize = 1;
				GDALRasterIOExtraArg arg;
				INIT_RASTERIO_EXTRA_ARG(arg);
				arg.eResampleAlg = GRIORA_NearestNeighbour;
				_aoiBand->RasterIO(GF_Read, curAoiBlockStartX, curAoiBlockStartY, curAoiBlockXSize, curAoiBlockYSize, _aoiBlockBuffer, blockInfo.xValidBlockSize, blockInfo.yValidBlockSize, GDT_Byte, 0, 0, &arg);
			}

			proc(user, 
				_band, 
				&blockInfo, _blockBuffer,
				_dataType, _dataBytes,
				_aoiBlockBuffer, _aoiNoDataValue,
				progress);
		}
	}
}

END_NAME_SPACE(tGis, Core)
