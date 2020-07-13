#pragma once

#ifndef __RASTERBANDSEQBLOCKREADER_H__
#define __RASTERBANDSEQBLOCKREADER_H__

#include "Helper.h"
#include "tGis_CoreCfg.h"

#include "gdal.h"
#include "gdal_priv.h"

#include "RasterBandStorageBlockGridReader.h"

BEGIN_NAME_SPACE(tGis, Core)

//遍历波段上每一个像素块， 通常用作卷积计算
//
//没有padding，步长是1，卷积计算的padding需要自行处理
//假如波段大小是4*4，请求遍历的像素块是3*3，那么总共需要遍历4次
//
class TGIS_CORE_API RasterBandPixelBlockWalker
{
public:
	// x,y为读取像素块左上角的像素坐标
	//
	//return 返回false不再继续遍历
	//
	typedef bool(__stdcall *FOREACHBLOCK_FUNC)(void* user, GDALRasterBand* band, void* block, int x, int y);

public:
	RasterBandPixelBlockWalker(GDALDataset* raster,int band,int width,int height,GDALDataset* aoiRaster = nullptr, int aoiBand=0,int mlimit = 16);
	RasterBandPixelBlockWalker(GDALDataset* raster,GDALRasterBand* band,int width,int height,GDALDataset* aoiRaster = nullptr, int aoiBand=0,int mlimit = 16);
	RasterBandPixelBlockWalker(GDALDataset* raster,int band,int width,int height,int xOffset = 0, int yOffset = 0, int xSize = -1, int ySize = -1,int mlimit = 16);
	RasterBandPixelBlockWalker(GDALDataset* raster,GDALRasterBand* band,int width,int height,int xOffset = 0, int yOffset = 0, int xSize = -1, int ySize = -1,int mlimit = 16);
	~RasterBandPixelBlockWalker(void);

	int GetXOffset() { return _xOffset; }

	int GetYOffset() { return _yOffset; }

	int GetXSize() { return _xSize; }

	int GetYSize() { return _ySize; }

	//重新开始遍历读取，新建之后的对象不用调用这个方法即可直接读取
	void ResetReading();

	void ForEachBlock(FOREACHBLOCK_FUNC proc,void* user);

private:
	GDALDataset* _raster;
	GDALRasterBand* _band;
	GDALDataType _dataType;
	int _dataBytes;

	int _xOffset;
	int _yOffset;
	int _xEnd;
	int _yEnd;
	int _xSize;
	int _ySize;
	int _xReadSize;
	int _yReadSize;

	GDALDataset* _aoiRaster;
	GDALRasterBand* _aoiBand;
	double _aoiNoDataValue;
	bool _aoiNoDataValueValid;

	int _xAoiOffset;
	int _yAoiOffset;
	int _xAoiEnd;
	int _yAoiEnd;
	int _xAoiSize;
	int _yAoiSize;

	double _xAoiPixScale;
	double _yAoiPixScale;

private:
	RasterBandStorageBlockGridReader* _aoiGridReader;
	RasterBandStorageBlockGridReader* _rasterGridReader;
};



END_NAME_SPACE(tGis, Core)


#endif
