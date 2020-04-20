#pragma once

#include "Helper.h"
#include "Event.h"
#include "Progress.h"

#include "gdal.h"
#include "gdal_priv.h"


BEGIN_NAME_SPACE(tGis, Core)

// sizeThreshold ���߳�����ֵ��������ֵʱ�Ŵ���������
// topLevelSize ������������������������ĳ��߳������ֵ
TGIS_CORE_API bool NeedBuildPyramids(GDALDataset * raster,
	int sizeThreshold = 1080,
	int topLevelSize = 255);


TGIS_CORE_API void BuildPyramids(GDALDataset * raster,
	ProgressEvent * progressEvent = nullptr,
	int sizeThreshold = 1080, 
	int topLevelSize = 255);

END_NAME_SPACE(tGis, Core)

