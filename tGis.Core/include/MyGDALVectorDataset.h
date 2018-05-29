#pragma once

#ifndef __MYGDALVECTORDATASET_H__
#define __MYGDALVECTORDATASET_H__

#include "Helper.h"

#include "IDataset.h"
#include "MyGDALFileDataset.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include <string>

using namespace std;



BEGIN_NAME_SPACE(tGis, Core)

class VectorSimpleLayerProvider;

class TGIS_API MyGDALVectorDataset : public MyGDALFileDataset
{
public:
	const char* GetType();
	static const char* S_GetType();

private:
	static const char* const _type;

public:
	MyGDALVectorDataset();
	MyGDALVectorDataset(const char* path, bool delayOpen = true, GDALAccess eAccess = GA_Update, bool autoClose = true);
	~MyGDALVectorDataset();

public:
	using MyGDALFileDataset::GetGDALDataset;
	using MyGDALFileDataset::Detach;
	using MyGDALFileDataset::SetAutoClose;
	using MyGDALFileDataset::GetAutoClose;

	void Attach(const char* file, GDALAccess eAccess, bool autoClose = true) override;
	void Attach(GDALDataset* dataset, bool autoClose = false) ;

private:
	CPL_DISALLOW_COPY_ASSIGN(MyGDALVectorDataset)
};




END_NAME_SPACE(tGis, Core)

#endif

