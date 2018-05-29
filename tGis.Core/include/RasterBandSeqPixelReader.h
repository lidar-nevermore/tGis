#pragma once

#ifndef __I_RASTERBANDSEQPIXELREADER_H__
#define __I_RASTERBANDSEQPIXELREADER_H__

#include "Helper.h"

#include "gdal.h"
#include "gdal_priv.h"


BEGIN_NAME_SPACE(tGis, Core)

typedef struct _StorageBlockBuffer
{
	//当前遍历至的块指针
	void* Buffer; 
	//xBlockOff,yBlockOff 当前所遍历块的编号，也就是第几个块
	int xBlockOff; 
	int yBlockOff;
	//xBlockSize,yBlockSize 当前所遍历块的像素尺寸
	int xBlockSize;
	int yBlockSize;
	//xBlock,yBlock 当前块左上角的 像素坐标
	int xBlockLTC;
	int yBlockLTC;
	//xValidBlockSize,yValidBlockSize 当前块中 处于AOI外接矩形中的部分 的像素尺寸
	int xValidBlockSize;
	int yValidBlockSize;
	//xStartPosIB,yStartPosIB 当前块中 处于AOI外接矩形中的部分 的左上角 在Block中的位置
	int xStartPosIB;
	int yStartPosIB;
	//xEndPosIB,yEndPosIB 当前块中 处于AOI外接矩形中的部分 的右下角 在Block中的位置
	int xEndPosIB;
	int yEndPosIB;
} StorageBlockBuffer;


class TGIS_API RasterBandSeqPixelReader
{
public:
	typedef void(*FOREACHPIXEL_FUNC)(void* user, GDALRasterBand* band, double pix, int x, int y, void* orgPix, StorageBlockBuffer* block, int xPosIB, int yPosIB);
	typedef void(*FOREACHBLOCK_FUNC)(void* user, GDALRasterBand* band, StorageBlockBuffer* block);

public:
	RasterBandSeqPixelReader(GDALDataset* raster,int band,GDALDataset* aoiRaster = nullptr, int aoiBand=0);
	RasterBandSeqPixelReader(GDALDataset* raster,GDALRasterBand* band,GDALDataset* aoiRaster = nullptr, int aoiBand=0);
	RasterBandSeqPixelReader(GDALDataset* raster,int band,int xOffset, int yOffset, int xSize, int ySize);
	RasterBandSeqPixelReader(GDALDataset* raster,GDALRasterBand* band,int xOffset, int yOffset, int xSize, int ySize);
	~RasterBandSeqPixelReader(void);

public:
	int GetXOffset() { return _xOffset; }

	int GetYOffset() { return _yOffset; }

	int GetXSize() { return _xSize; }

	int GetYSize() { return _ySize; }

	void ResetPixelReading();

	//遍历像素; raster 当前遍历的影像 band 当前遍历的波段 pix 像素值 x,y 像素的图像坐标 orgPix 原始像素值指针   xPos,yPos 当前遍历至像素在块中坐标
	void ForEachPixel(FOREACHPIXEL_FUNC proc, void* user);

	void ResetBlockReading();

	//遍历像素块; 
	void ForEachBlock(FOREACHBLOCK_FUNC proc, void* user);

private:
	int _xOffset;
	int _yOffset;
	int _xEnd;
	int _yEnd;
	int _xSize;
	int _ySize;

	int _xBlockSize;
	int _yBlockSize;

	unsigned char* _blockBuffer;

	GDALDataType _dataType;
	int _dataBytes;

	GDALDataset* _raster;
	GDALRasterBand* _band;

	int _xAoiOffset;
	int _yAoiOffset;
	int _xAoiEnd;
	int _yAoiEnd;
	int _xAoiSize;
	int _yAoiSize;

	double _xAoiPixScale;
	double _yAoiPixScale;

	GDALDataset* _aoiRaster;
	GDALRasterBand* _aoiBand;
	double _aoiNoDataValue;
	bool _aoiNoDataValueValid;

	unsigned char* _aoiBlockBuffer;


private:
	int _startXBlockOff; //所有需遍历块中左上角块的水平索引
	int _startYBlockOff; //所有需遍历块中左上角块的竖直索引
	int _endXBlockOff; //所有需遍历块中右下角块的水平索引
	int _endYBlockOff; //所有需遍历块中右下角块的竖直索引

	int _curBlockX; //当前块左上角X像素坐标
	int _curBlockY; //当前块左上角Y像素坐标

	int _curStartXPosIB; //当前起始遍历像素在Block中的X位置
	int _curStartYPosIB; //当前起始遍历像素在Block中的Y位置
	int _curEndXPosIB; //当前结束遍历像素在Block中的X位置
	int _curEndYPosIB; //当前结束遍历像素在Block中的Y位置

	void Init(GDALDataset* raster,GDALRasterBand* band,int xOffset, int yOffset, int xEnd, int yEnd);

private:
	void CalcPixelTraverseParam(int xBlockOff,int yBlockOff,int xBlockSize,int yBlockSize);

	double GetPixelValue(void* pix);

private:
    CPL_DISALLOW_COPY_ASSIGN(RasterBandSeqPixelReader)

};



END_NAME_SPACE(tGis, Core)


#endif

