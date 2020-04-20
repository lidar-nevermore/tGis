#pragma once

#include "Helper.h"
#include "Event.h"
#include "Progress.h"

#include "gdal.h"
#include "gdal_priv.h"


BEGIN_NAME_SPACE(tGis, Core)

// sizeThreshold 长边长的阈值，大于阈值时才创建金字塔
// topLevelSize 如果创建金字塔，最顶层金字塔的长边长的最大值
TGIS_CORE_API bool NeedBuildPyramids(GDALDataset * raster,
	int sizeThreshold = 1080,
	int topLevelSize = 255);


TGIS_CORE_API void BuildPyramids(GDALDataset * raster,
	ProgressEvent * progressEvent = nullptr,
	int sizeThreshold = 1080, 
	int topLevelSize = 255);

END_NAME_SPACE(tGis, Core)

