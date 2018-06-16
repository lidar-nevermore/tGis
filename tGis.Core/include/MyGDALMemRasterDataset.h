#pragma once

#ifndef __MYGDALMEMRASTERDATASET_H__
#define __MYGDALMEMRASTERDATASET_H__

#include "Helper.h"

#include "IDataset.h"
#include "MyGDALRasterDataset.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include <string>

using namespace std;



BEGIN_NAME_SPACE(tGis, Core)

class TGIS_API MyGDALMemRasterDataset
{
public:
	MyGDALMemRasterDataset();
	~MyGDALMemRasterDataset();
};


END_NAME_SPACE(tGis, Core)

#endif

