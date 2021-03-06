#pragma once

#ifndef __MYGDALVECTORDATASET_H__
#define __MYGDALVECTORDATASET_H__

#include "Helper.h"

#include "MyGDALDataset.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"

BEGIN_NAME_SPACE(tGis, Core)


class TGIS_CORE_API MyGDALVectorDataset : public MyGDALDataset
{
	friend class FileSystemDataSource;
public:
	const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);

private:
	static const char* const _type;

public:
	MyGDALVectorDataset(DataSource* ds, const char* path, GDALAccess eAccess = GA_Update, bool delayOpen = true, bool autoClose = true);
	MyGDALVectorDataset();
	virtual ~MyGDALVectorDataset();

public:
	virtual void Open();
	virtual void Close();

public:
	using MyGDALDataset::GetGDALDataset;
	using MyGDALDataset::Detach;
	using MyGDALDataset::SetAutoClose;
	using MyGDALDataset::GetAutoClose;

	void Attach(const char* file, GDALAccess eAccess, bool autoClose = true);
	void Attach(GDALDataset* dataset, bool autoClose = false);

private:
	CPL_DISALLOW_COPY_ASSIGN(MyGDALVectorDataset)
};


END_NAME_SPACE(tGis, Core)

#endif

