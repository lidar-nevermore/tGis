
#include "RasterBandSeqBlockReader.h"
#include "MyGDALRasterDataset.h"
#include <cassert>
#include <algorithm>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

RasterBandSeqBlockReader::~RasterBandSeqBlockReader(void)
{
	if(_aoiGridReader != nullptr)
		delete _aoiGridReader;
	if(_rasterGridReader != nullptr)
		delete _rasterGridReader;
}

RasterBandSeqBlockReader::RasterBandSeqBlockReader( GDALDataset* raster,int band,int width,int height,GDALDataset* aoiRaster /*= nullptr*/, int aoiBand/*=0*/,int mlimit /*= 16*/ )
{
	new (this)RasterBandSeqBlockReader(raster,raster->GetRasterBand(band),width,height,aoiRaster,aoiBand,mlimit);
}

RasterBandSeqBlockReader::RasterBandSeqBlockReader( GDALDataset* raster,GDALRasterBand* band,int width,int height,GDALDataset* aoiRaster /*= nullptr*/, int aoiBand/*=0*/,int mlimit /*= 16*/ )
{
	int xRasterSize = raster->GetRasterXSize();
	int yRasterSize = raster->GetRasterYSize();

	_raster = raster;
	_band = band;
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

	_xSize = xRasterSize;
	_ySize = yRasterSize;
	_xOffset = 0;
	_yOffset = 0;
	_xEnd = xRasterSize;
	_yEnd = yRasterSize;
	_xReadSize = width;
	_yReadSize = height;

	_aoiRaster = nullptr;
	_aoiBand = nullptr;
	_aoiNoDataValue = INT_MIN;
	_aoiNoDataValueValid = false;

	_xAoiOffset = _xOffset;
	_yAoiOffset = _yOffset;
	_xAoiEnd = _xEnd;
	_yAoiEnd = _yEnd;
	_xAoiSize = _xSize;
	_yAoiSize = _ySize;

	_xAoiPixScale = 1;
	_yAoiPixScale = 1;
	
	_aoiGridReader = nullptr;

	if (aoiRaster == nullptr)
	{
		_rasterGridReader = new RasterBandStorageBlockGridReader(_band,width,height,_xOffset,_yOffset,_xEnd,_yEnd,mlimit);
	}
    else //if(aoiRaster != nullptr)
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

		myInRaster.Spatial2Pixel(aoienvelope.MinX, aoienvelope.MinY, &minInPixX, &minInPixY);
		myInRaster.Spatial2Pixel(aoienvelope.MaxX, aoienvelope.MaxY, &maxInPixX, &maxInPixY);

		_xAoiOffset = my_round(min(minInPixX, maxInPixX), 0);
		_yAoiOffset = my_round(min(minInPixY, maxInPixY), 0);
		_xAoiEnd = my_round(max(minInPixX, maxInPixX) + 1, 0);
		_yAoiEnd = my_round(max(minInPixY, maxInPixY) + 1, 0);

		if(_xOffset < 0)
			_xOffset = 0;
		if(_yOffset < 0)
			_yOffset = 0;

		if(_xEnd > xRasterSize)
			_xEnd = xRasterSize;
		if(_yEnd > yRasterSize)
			_yEnd = yRasterSize;

		_xSize = _xEnd-_xOffset;
		_ySize = _yEnd-_yOffset;

		double minAoiPixX = 0;
		double minAoiPixY = 0;
		double maxAoiPixX = 0;
		double maxAoiPixY = 0;
		int xAoiRasterSize = _aoiRaster->GetRasterXSize();
		int yAoiRasterSize = _aoiRaster->GetRasterYSize();

		myAoiRaster.Spatial2Pixel(aoienvelope.MinX,aoienvelope.MinY,&minAoiPixX,&minAoiPixY);
		myAoiRaster.Spatial2Pixel(aoienvelope.MaxX,aoienvelope.MaxY,&maxAoiPixX,&maxAoiPixY);

		_xAoiOffset = my_round(min(minAoiPixX,maxAoiPixX), 0);
		_yAoiOffset = my_round(min(minAoiPixY,maxAoiPixY), 0);
		_xAoiEnd = my_round(max(minAoiPixX,maxAoiPixX)+1, 0);
		_yAoiEnd = my_round(max(minAoiPixY,maxAoiPixY)+1, 0);

		if(_xAoiOffset < 0)
			_xAoiOffset = 0;
		if(_yAoiOffset < 0)
			_yAoiOffset = 0;

		if(_xAoiEnd > xAoiRasterSize)
			_xAoiEnd = xAoiRasterSize;
		if(_yAoiEnd > yAoiRasterSize)
			_yAoiEnd = yAoiRasterSize;

		_xAoiSize = _xAoiEnd-_xAoiOffset;
		_yAoiSize = _yAoiEnd-_yAoiOffset;

		_xAoiPixScale = (double)_xAoiSize/_xSize;
		_yAoiPixScale = (double)_yAoiSize/_ySize;
	
		_rasterGridReader = new RasterBandStorageBlockGridReader(_band,width,height,_xOffset,_yOffset,_xEnd,_yEnd,mlimit);

		int xReadSize = _rasterGridReader->GetStorageBlockGridEndX()-_rasterGridReader->GetStorageBlockGridBeginX();
		int yReadSize = _rasterGridReader->GetStorageBlockGridEndY()-_rasterGridReader->GetStorageBlockGridBeginY();

		int hfxReadSize = width/2;
		int hfyReadSize = height/2;

		int xAoiReadSize = ((xReadSize - hfxReadSize - hfxReadSize)*_xAoiSize + _xSize - 1)/_xSize;
		int yAoiReadSize = ((yReadSize - hfyReadSize - hfyReadSize)*_yAoiSize + _ySize - 1)/_ySize;

		if(xAoiReadSize < 1)
			xAoiReadSize = 1;
		if(yAoiReadSize < 1)
			yAoiReadSize = 1;

		if(xAoiReadSize > xAoiRasterSize)
			xAoiReadSize = xAoiRasterSize;
		if(yAoiReadSize > yAoiRasterSize)
			yAoiReadSize = yAoiRasterSize;

		_aoiGridReader = new RasterBandStorageBlockGridReader(_aoiBand,xAoiReadSize,yAoiReadSize,_xAoiOffset,_yAoiOffset,_xAoiEnd,_yAoiEnd,mlimit);
		_aoiGridReader->SetReadingBlockSize(1,1);
	}

}

RasterBandSeqBlockReader::RasterBandSeqBlockReader( GDALDataset* raster,int band,int width,int height,int xOffset /*= 0*/, int yOffset /*= 0*/, int xSize /*= -1*/, int ySize /*= -1*/,int mlimit /*= 16*/ )
{
	new (this)RasterBandSeqBlockReader(raster,raster->GetRasterBand(band),width,height,xOffset,yOffset, xSize,ySize,mlimit);
}

RasterBandSeqBlockReader::RasterBandSeqBlockReader( GDALDataset* raster,GDALRasterBand* band,int width,int height,int xOffset /*= 0*/, int yOffset /*= 0*/, int xSize /*= -1*/, int ySize /*= -1*/,int mlimit /*= 16*/ )
{
	int xRasterSize = raster->GetRasterXSize();
	int yRasterSize = raster->GetRasterYSize();

	_raster = raster;
	_band = band;
	_dataType = _band->GetRasterDataType();
	_dataBytes = GDALGetDataTypeSizeBytes(_dataType);

	_xSize = xSize;
	_ySize = ySize;
	_xOffset = xOffset;
	_yOffset = yOffset;
	_xEnd = _xOffset + _xSize;
	_yEnd = _yOffset + _ySize;
	_xReadSize = width;
	_yReadSize = height;

	assert(_xEnd <= xRasterSize);
	assert(_yEnd <= yRasterSize);

	_aoiRaster = nullptr;
	_aoiBand = nullptr;
	_aoiNoDataValue = INT_MIN;
	_aoiNoDataValueValid = false;

	_xAoiOffset = _xOffset;
	_yAoiOffset = _yOffset;
	_xAoiEnd = _xEnd;
	_yAoiEnd = _yEnd;
	_xAoiSize = _xSize;
	_yAoiSize = _ySize;

	_xAoiPixScale = 1;
	_yAoiPixScale = 1;

	_aoiGridReader = nullptr;
	_rasterGridReader = new RasterBandStorageBlockGridReader(_band,width,height,_xOffset,_yOffset,_xEnd,_yEnd,mlimit);
}

void RasterBandSeqBlockReader::ResetReading()
{
	_rasterGridReader->ResetReading();
	if (_aoiGridReader != nullptr)
	{
		_aoiGridReader->ResetReading();
	}
}

void RasterBandSeqBlockReader::ForEachBlock(FOREACHBLOCK_FUNC proc, void* user)
{
	int m = RasterBandStorageBlockGridReader::MOV_NONE;

	if (_rasterGridReader->GetMajorOrder() == RasterBandStorageBlockGridReader::ORDER_COL)
	{
		m = RasterBandStorageBlockGridReader::MOV_DOWN;
	}
	else
	{
		m = RasterBandStorageBlockGridReader::MOV_RIGHT;
	}

	int xReadBegin = _xOffset;
	int yReadBegin = _yOffset;

	int xReadEnd = _rasterGridReader->GetStorageBlockGridEndX()-_xReadSize+1;
	int yReadEnd = _rasterGridReader->GetStorageBlockGridEndY()-_yReadSize+1;

	int xFinalReadEnd = _xEnd-_xReadSize+1;
	int yFinalReadEnd = _yEnd-_yReadSize+1;

	xReadEnd = xReadEnd<xFinalReadEnd?xReadEnd:xFinalReadEnd;
	yReadEnd = yReadEnd<yFinalReadEnd?yReadEnd:yFinalReadEnd;

	do 
	{
		for (int y=yReadBegin;y<yReadEnd;y++)
		{
			for (int x=xReadBegin; x<xReadEnd; x++)
			{
				void* block = _rasterGridReader->GetOneReadingBlock(x,y);

				if (_aoiGridReader == nullptr)
				{
					proc(user,_band,block,x,y);
				}
				else
				{
					int xAoi = (int)my_round((double)(x-_xOffset)*_xAoiSize/_xSize + (double)(_xReadSize+1)*_xAoiSize/(2*_xSize),0);
					int yAoi = (int)my_round((double)(y-_yOffset)*_yAoiSize/_ySize + (double)(_yReadSize+1)*_yAoiSize/(2*_ySize),0);
					if (!_aoiGridReader->ContainReadingBlock(xAoi,yAoi))
					{
						if(_aoiGridReader->MoveArbitrarily(xAoi,yAoi,m) == false)
						{
							int x = 0;
						}
					}

					unsigned char* aoi = (unsigned char*)_aoiGridReader->GetOneReadingBlock(xAoi,yAoi);
					if (*aoi > 0 && *aoi != _aoiNoDataValue)
					{
						proc(user,_band,block,x,y);
					}
				}				
			}
		}

		m = _rasterGridReader->MoveNext();

		if (m == RasterBandStorageBlockGridReader::MOV_DOWN)
		{
			yReadBegin = yReadEnd;
			yReadEnd = _rasterGridReader->GetStorageBlockGridEndY()-_yReadSize+1;
			yReadEnd = yReadEnd<yFinalReadEnd?yReadEnd:yFinalReadEnd;
		}
		else if (m == RasterBandStorageBlockGridReader::MOV_UP)
		{
			yReadEnd = yReadBegin;
			yReadBegin = _rasterGridReader->GetStorageBlockGridBeginY();
			yReadBegin = yReadBegin>_yOffset?yReadBegin:_yOffset;
		}
		else if (m == RasterBandStorageBlockGridReader::MOV_LEFT)
		{
			xReadEnd = xReadBegin;
			xReadBegin = _rasterGridReader->GetStorageBlockGridBeginX();
			xReadBegin = xReadBegin>_xOffset?xReadBegin:_xOffset;
		}
		else if (m == RasterBandStorageBlockGridReader::MOV_RIGHT)
		{
			xReadBegin = xReadEnd;
			xReadEnd = _rasterGridReader->GetStorageBlockGridEndX()-_xReadSize+1;
			xReadEnd = xReadEnd<xFinalReadEnd?xReadEnd:xFinalReadEnd;
		}
	} while (m != RasterBandStorageBlockGridReader::MOV_NONE);
}

END_NAME_SPACE(tGis, Core)
