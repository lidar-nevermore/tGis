#pragma once
#include "gdal.h"
#include "cpl_conv.h"
#include "gdal_priv.h"
#include "ogrsf_frmts.h"
#include <functional>

#include "tGisCore.h"

using namespace tGis::Core;

class RasterBinarize
{
private:
	MyGDALFileRasterDataset _inRasterWraper;
	GDALDataset* _inRaster;
	GDALRasterBand* _inBand;

	MyGDALFileRasterDataset _aoiRasterWraper;
	GDALDataset* _aoiRaster;
	int _aoiBand;

	unsigned char* _outBuffer;
	int _outBufferSize;
	int _xBufferSize;
	int _yBufferSize;

private:
	void Init();

public:
	RasterBinarize(const char* inRasterPath,const int band,const char* aoiRasterPath,const int aoiBand);
	RasterBinarize(GDALDataset* inRaster,const int band,GDALDataset* aoiRaster,const int aoiBand);
	~RasterBinarize(void);

public:
	int Process(const char* outRasterPath,const unsigned char fPixValue,const std::function<bool(double pix)>& tester);
};

