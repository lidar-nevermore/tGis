#pragma once

#ifndef __RASTERWRITER_H__
#define __RASTERWRITER_H__

#include "Helper.h"
#include "Event.h"

#pragma warning( disable: 4251 )
#include "gdal.h"
#include "gdal_priv.h"

BEGIN_NAME_SPACE(tGis, Core)


TGIS_API void WriteMemoryBlock(
	const char* path, 
	void** mem, GDALDataType dt, int w, int h, int count = 0,
	ProgressEvent * progressEvent = nullptr,
	GDALDataset * raster = nullptr, int xOffset = 0, int yOffset = 0);


TGIS_API void CopyRaster(
	const char* path, 
	GDALDataset * raster,
	int xOffset, int yOffset, int xSize, int ySize,
	int dstXSize, int dstYSize,
	int* bands = nullptr, int count = 0,
	ProgressEvent * progressEvent = nullptr);

END_NAME_SPACE(tGis, Core)


#endif

