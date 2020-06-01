#pragma once

#ifndef __RASTERBANDSEQPIXELREADER_H__
#define __RASTERBANDSEQPIXELREADER_H__

#include "Helper.h"
#include "tGis_CoreCfg.h"

#include "gdal.h"
#include "gdal_priv.h"


BEGIN_NAME_SPACE(tGis, Core)

//影像一般会按块进行存储，每个存储块占据连续的地址空间
//调用readblock接口一次读取一整块，具有更高的读写效率
struct StorageBlockInfo
{
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
} ;


class TGIS_CORE_API RasterBandStorageBlockWalker
{
public:
	//遍历像素回调
	//band 当前遍历的波段 
	//pix 像素值 
	//xIM, yIM 像素的图像坐标 
	//xIB, yIB 当前遍历至像素在块中坐标 
	//orgPix 原始像素值指针(指向blockBuffer中某处，具体类型由band的DataType确定) 
	//blockInfo 当前块信息 
	//blockBuffer 当前块内存 
	//dataType 波段的数据类型
	//dataBytes 波段数据占据的字节数目
	//progress 遍历进度 多个块更新一次进度，如果进度值没有改变不需要更新ui上的进度提示，更新进度提示会拖累效率
	//
	//functor或者lambda表达式调用比直接函数指针调用要慢很多
	//不推荐用该接口
	typedef void(__stdcall *FOREACHPIXEL_FUNC)(void* user, 
		GDALRasterBand* band,
		double pix, int xIM, int yIM, int xIB, int yIB, void* orgPix,
		StorageBlockInfo* blockInfo, void* blockBuffer,
		GDALDataType dataType, int dataBytes,
		double progress);

	//遍历像素块回调
	//blockInfo 当前块信息 
	//blockBuffer 当前块内存 
	//dataType 波段的数据类型
	//dataBytes 波段数据占据的字节数目
	//aoiBuffer 当前块中在AOI范围内的部分对应的AOI像素缓存
	//aoiNoDataValue aoi像素的无效值，没有无效值将会传递INT_MIN
	//progress 遍历进度 多个块更新一次进度，如果进度值没有改变不需要更新ui上的进度提示，更新进度提示会拖累效率
	typedef void(__stdcall *FOREACHBLOCK_FUNC)(void* user,
		GDALRasterBand* band,
		StorageBlockInfo* blockInfo, void* blockBuffer,
		GDALDataType dataType, int dataBytes,
		unsigned char* aoiBuffer, double aoiNoDataValue,
		double progress);

public:
	RasterBandStorageBlockWalker(GDALDataset* raster,int band,GDALDataset* aoiRaster = nullptr, int aoiBand=0);
	RasterBandStorageBlockWalker(GDALDataset* raster,GDALRasterBand* band,GDALDataset* aoiRaster = nullptr, int aoiBand=0);
	RasterBandStorageBlockWalker(GDALDataset* raster,int band,int xOffset, int yOffset, int xSize, int ySize);
	RasterBandStorageBlockWalker(GDALDataset* raster,GDALRasterBand* band,int xOffset, int yOffset, int xSize, int ySize);
	~RasterBandStorageBlockWalker(void);

public:
	int GetXOffset() { return _xOffset; }

	int GetYOffset() { return _yOffset; }

	int GetXSize() { return _xSize; }

	int GetYSize() { return _ySize; }

	int GetXBlockSize() { return _xBlockSize; }

	int GetYBlockSize() { return _yBlockSize; }

	void ResetPixelReading();

	//遍历像素
	void ForEachPixel(FOREACHPIXEL_FUNC proc, void* user);

	void ResetBlockReading();

	//遍历像素块; 
	void ForEachBlock(FOREACHBLOCK_FUNC proc, void* user);

protected:
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

private:
    CPL_DISALLOW_COPY_ASSIGN(RasterBandStorageBlockWalker)

};



END_NAME_SPACE(tGis, Core)


#endif

