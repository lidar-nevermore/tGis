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


class TGISCORE_API MyGDALVectorDataset : public MyGDALFileDataset
{
public:
	const char* GetType();

private:
	static const char* const _type;

private:
	OGREnvelope _envelope;

public:
	MyGDALVectorDataset();
	MyGDALVectorDataset(const char* path, bool delayOpen = true, GDALAccess eAccess = GA_Update, bool autoClose = true);
	virtual ~MyGDALVectorDataset();

public:

	void Attach(GDALDataset* dataset, bool autoClose = false);

private:
	CPL_DISALLOW_COPY_ASSIGN(MyGDALVectorDataset)
};




END_NAME_SPACE(tGis, Core)

#endif

