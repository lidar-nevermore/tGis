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

class TGIS_CORE_API MyGDALMemRasterDataset : public MyGDALRasterDataset
{
public:
	const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);

private:
	static const char* const _type;

public:
	MyGDALMemRasterDataset(IDataSource* ds = nullptr, const char* name = nullptr);

public:
	~MyGDALMemRasterDataset();

public:
	virtual void Open();

public:
	using MyGDALRasterDataset::GetGDALDataset;
	using MyGDALRasterDataset::Detach;
	using MyGDALRasterDataset::SetAutoClose;
	using MyGDALRasterDataset::GetAutoClose;
	//需要判断dataset是否是存在于内存的
	void Attach(GDALDataset* dataset, bool autoClose = false, double noDataValue = NAN);


private:
	CPL_DISALLOW_COPY_ASSIGN(MyGDALMemRasterDataset)
};


END_NAME_SPACE(tGis, Core)

#endif

