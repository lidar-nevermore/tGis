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

class TGIS_API MyGDALFileRasterDataset : public MyGDALRasterDataset
{
	friend class FileSystemDataSource;
public:
	const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);
	virtual bool IsTypeOf(ITGisObject* object);
	virtual void Open();

private:
	static const char* const _type;

protected:
	MyGDALFileRasterDataset(IDataSource* ds, const char* path, GDALAccess eAccess = GA_Update, bool delayOpen = true, bool autoClose = true);

public:
	MyGDALFileRasterDataset();
	~MyGDALFileRasterDataset();

public:
	using MyGDALRasterDataset::GetGDALDataset;
	using MyGDALRasterDataset::Detach;
	using MyGDALRasterDataset::SetAutoClose;
	using MyGDALRasterDataset::GetAutoClose;
	using MyGDALRasterDataset::Attach;

	void Attach(const char* file, GDALAccess eAccess, bool autoClose = true, double noDataValue = NAN);

	void AttachHDF(const char* file, GDALAccess eAccess, const int subdataset, bool autoClose = true, double noDataValue = NAN);
};


END_NAME_SPACE(tGis, Core)

#endif

