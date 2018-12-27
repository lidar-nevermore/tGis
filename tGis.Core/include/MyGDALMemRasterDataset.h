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
	friend class MemoryDataSourceProvider;
public:
	const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);
	virtual bool IsTypeOf(ITGisObject* object);
	virtual void Open();

private:
	static const char* const _type;

protected:
	MyGDALMemRasterDataset(IDataSource* ds = nullptr, const char* name = nullptr);

public:
	~MyGDALMemRasterDataset();

public:
	using MyGDALRasterDataset::GetGDALDataset;
	using MyGDALRasterDataset::Detach;
	using MyGDALRasterDataset::SetAutoClose;
	using MyGDALRasterDataset::GetAutoClose;
	using MyGDALRasterDataset::Attach;

private:
	CPL_DISALLOW_COPY_ASSIGN(MyGDALMemRasterDataset)
};


END_NAME_SPACE(tGis, Core)

#endif

