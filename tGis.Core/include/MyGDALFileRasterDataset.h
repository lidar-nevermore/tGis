#pragma once

#ifndef __MYGDALFILERASTERDATASET_H__
#define __MYGDALFILERASTERDATASET_H__

#include "Helper.h"

#include "IDataset.h"
#include "MyGDALRasterDataset.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include <string>

using namespace std;



BEGIN_NAME_SPACE(tGis, Core)

class TGIS_CORE_API MyGDALFileRasterDataset : public MyGDALRasterDataset
{
	friend class FileSystemDataSource;
public:
	const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);

private:
	static const char* const _type;

public:
	MyGDALFileRasterDataset();
	MyGDALFileRasterDataset(IDataSource* ds, const char* path, GDALAccess eAccess = GA_Update, bool delayOpen = true, bool autoClose = true);
	~MyGDALFileRasterDataset();

public:
	virtual void Open();

public:
	using MyGDALRasterDataset::GetGDALDataset;
	using MyGDALRasterDataset::Detach;
	using MyGDALRasterDataset::SetAutoClose;
	using MyGDALRasterDataset::GetAutoClose;

	//需要判断dataset是否是来自文件的
	void Attach(GDALDataset* dataset, bool autoClose = false, double noDataValue = NAN);

	void Attach(const char* file, GDALAccess eAccess, bool autoClose = true, double noDataValue = NAN);

	void AttachHDF(const char* file, GDALAccess eAccess, const int subdataset, bool autoClose = true, double noDataValue = NAN);
};


END_NAME_SPACE(tGis, Core)

#endif

