#pragma once

#ifndef __MultiRasterBandStorageBlockWalker_H__
#define __MultiRasterBandStorageBlockWalker_H__


#include "Helper.h"
#include "tGis_CoreCfg.h"

#include "gdal.h"
#include "gdal_priv.h"

#include "RasterBandStorageBlockWalker.h"

BEGIN_NAME_SPACE(tGis, Core)

class MultiRasterBandStorageBlockWalker : public RasterBandStorageBlockWalker
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
	typedef void(__stdcall *M_FOREACHPIXEL_FUNC)(void* user,
		GDALRasterBand** band,
		double* pix, int xIM, int yIM, int xIB, int yIB, void** orgPix,
		StorageBlockInfo* blockInfo, void** blockBuffer,
		GDALDataType* bandDataType, int* bandDataBytes,
		double progress);

	//遍历像素块回调
	//blockInfo 当前块信息 
	//blockBuffer 当前块内存 
	//dataType 波段的数据类型
	//dataBytes 波段数据占据的字节数目
	//aoiBuffer 当前块中在AOI范围内的部分对应的AOI像素缓存
	//aoiNoDataValue aoi像素的无效值，没有无效值将会传递INT_MIN
	//progress 遍历进度 多个块更新一次进度，如果进度值没有改变不需要更新ui上的进度提示，更新进度提示会拖累效率
	typedef void(__stdcall *M_FOREACHBLOCK_FUNC)(void* user,
		GDALRasterBand** band,
		StorageBlockInfo* blockInfo, void** blockBuffer,
		GDALDataType* bandDataType, int* bandDataBytes,
		unsigned char* aoiBuffer, double aoiNoDataValue,
		double progress);

public:
	MultiRasterBandStorageBlockWalker(GDALDataset* raster, int* bands, int bandCount, GDALDataset* aoiRaster = nullptr, int aoiBand=0);
	~MultiRasterBandStorageBlockWalker(void);

private:
	static void __stdcall FOREACHPIXEL_OneBand(void* user,
		GDALRasterBand* band,
		StorageBlockInfo* blockInfo, void* blockBuffer,
		GDALDataType dataType, int dataBytes,
		unsigned char* aoiBuffer, double aoiNoDataValue,
		double progress);

	static void __stdcall FOREACHBLOCK_OneBand(void* user,
		GDALRasterBand* band,
		StorageBlockInfo* blockInfo, void* blockBuffer,
		GDALDataType dataType, int dataBytes,
		unsigned char* aoiBuffer, double aoiNoDataValue,
		double progress);

public:
	int GetBandCount() { return _bandCount; }

	void ForEachPixel(M_FOREACHPIXEL_FUNC proc, void* user);

	void ForEachBlock(M_FOREACHBLOCK_FUNC proc, void* user);

private:
	int _bandCount;
	//各波段块缓存
	void** _blockBuffer;
	//各波段是否可以用按块读取
	bool* _blockReading;
	//各波段指针
	GDALRasterBand** _bands;
	//各波段数据类型
	GDALDataType* _bandDataType;
	//各波段数据尺寸
	int* _bandDataBytes;
	//遍历像素时各波段像素
	double* _blockPixel;
	//用于遍历像素时在blockBuffer中移动
	char** _blockBufferPos;

private:
    CPL_DISALLOW_COPY_ASSIGN(MultiRasterBandStorageBlockWalker)

};

END_NAME_SPACE(tGis, Core)

#endif