#pragma once

#ifndef __MYGDALMEMRASTERDATASET_H__
#define __MYGDALMEMRASTERDATASET_H__

#include "Helper.h"

#include "MyGDALRasterDataset.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"


BEGIN_NAME_SPACE(tGis, Core)

class TGIS_CORE_API MyGDALMemRasterDataset : public MyGDALRasterDataset
{
public:
	const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);

private:
	static const char* const _type;

public:
	MyGDALMemRasterDataset(DataSource* ds, 
		MyGDALRasterDataset* raster, 
		int left, int top, int width, int height, 
		int *bandIndexs, int bandCount, 
		const char* name);

public:
	~MyGDALMemRasterDataset();

public:
	using MyGDALRasterDataset::GetGDALDataset;
	using MyGDALRasterDataset::Detach;
	using MyGDALRasterDataset::SetAutoClose;
	using MyGDALRasterDataset::GetAutoClose;

private:
	CPL_DISALLOW_COPY_ASSIGN(MyGDALMemRasterDataset)
};


END_NAME_SPACE(tGis, Core)

#endif

