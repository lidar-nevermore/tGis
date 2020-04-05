#pragma once

#ifndef __RASTERBANDSEQBLOCKREADER_H__
#define __RASTERBANDSEQBLOCKREADER_H__

#include "Helper.h"
#include "tGis_CoreCfg.h"

#include "gdal.h"
#include "gdal_priv.h"

#include "RasterBandStorageBlockGridReader.h"

BEGIN_NAME_SPACE(tGis, Core)

//按块读取像素，一般用来做卷积计算用
class TGIS_CORE_API RasterBandSeqBlockReader
{
public:
	// x,y为读取像素块左上角的像素坐标
	typedef void(*FOREACHBLOCK_FUNC)(void* user, GDALRasterBand* band, void* block, int x, int y);

public:
	RasterBandSeqBlockReader(GDALDataset* raster,int band,int width,int height,GDALDataset* aoiRaster = nullptr, int aoiBand=0,int mlimit = 16);
	RasterBandSeqBlockReader(GDALDataset* raster,GDALRasterBand* band,int width,int height,GDALDataset* aoiRaster = nullptr, int aoiBand=0,int mlimit = 16);
	RasterBandSeqBlockReader(GDALDataset* raster,int band,int width,int height,int xOffset = 0, int yOffset = 0, int xSize = -1, int ySize = -1,int mlimit = 16);
	RasterBandSeqBlockReader(GDALDataset* raster,GDALRasterBand* band,int width,int height,int xOffset = 0, int yOffset = 0, int xSize = -1, int ySize = -1,int mlimit = 16);
	~RasterBandSeqBlockReader(void);

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
