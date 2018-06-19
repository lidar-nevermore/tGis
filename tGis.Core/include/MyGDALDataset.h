#pragma once

#ifndef __MYGDALDATASET_H__
#define __MYGDALDATASET_H__

#include "Helper.h"

#include "Dataset.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include <string>
#include <vector>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

struct GDALInit;
struct IDataSource;

class TGIS_API MyGDALDataset : public Dataset
{
public:
	virtual const char* GetName();
	virtual const char* GetCreationString();

	virtual bool IsReadOnly();
	virtual bool IsOpened();
	virtual void Close();
	virtual IDataSource* GetDataSource();
	virtual const OGRSpatialReference* GetSpatialReference();
	virtual const OGREnvelope* GetEnvelope();

protected:
	OGRSpatialReference* _spatialRef;
	OGREnvelope _envelope;

protected:
	string _path;
	string _name;

private:
	static GDALInit* _GDALInit;

public:
	static int GetSupportedRasterFormatCount();
	static const vector<string>& GetSupportedRasterFormatExt(int);
	static const char* GetSupportedRasterFormatName(int);
	static bool GetSupportedRasterFormatCreatable(int);
	static bool IsSupportedRasterFormatExt(const char*);

	static int GetSupportedVectorFormatCount();
	static const vector<string>& GetSupportedVectorFormatExt(int);
	static const char* GetSupportedVectorFormatName(int);
	static bool GetSupportedVectorFormatCreatable(int);
	static bool IsSupportedVectorFormatExt(const char*);

protected:
	GDALDataset* _dataset;
	bool _autoClose;
	GDALAccess _eAccess;

public:
	MyGDALDataset(IDataSource* ds);
	virtual ~MyGDALDataset();

public:
	GDALDataset* GetGDALDataset();

protected:
	void Attach(const char* file, GDALAccess eAccess, bool autoClose = true);

	void Detach();

	void SetAutoClose(bool autoClose);

	bool GetAutoClose();
};

END_NAME_SPACE(tGis, Core)


#endif


