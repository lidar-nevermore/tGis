#pragma once
#include "gdal.h"
#include "cpl_conv.h"
#include "gdal_priv.h"
#include "ogrsf_frmts.h"

#include "tGisCore.h"

using namespace tGis::Core;

class RasterSubset
{
private:
	MyGDALFileRasterDataset _inRasterWraper;
	GDALDataset* _inRaster;

	int _xOffset;
	int _yOffset;
	int _xSize;
	int _ySize;

public:
	RasterSubset(GDALDataset* raster,int xOffset, int yOffset, int xSize, int ySize);
	~RasterSubset(void);

	void Process(const char* outRasterPath);
};

