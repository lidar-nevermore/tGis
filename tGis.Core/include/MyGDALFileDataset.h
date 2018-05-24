#pragma once

#ifndef __MYGDALDATASET_H__
#define __MYGDALDATASET_H__

#include "Helper.h"

#include "IDataset.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include <string>
#include <vector>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

struct GDALInit;

class TGISCORE_API MyGDALFileDataset : public IDataset
{
	friend class FileSystemDataSource;

public:
	virtual const char* GetName();
	virtual const char* GetCreationString();

	virtual bool IsOpened();
	virtual void Open();
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
	IDataSource* _dataSource;

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
	MyGDALFileDataset();
	virtual ~MyGDALFileDataset();

public:

	GDALDataset* GetGDALDataset();

	virtual void Attach(const char* file, GDALAccess eAccess, bool autoClose = true) = 0;

	void Detach();

	void SetAutoClose(bool autoClose);

	bool GetAutoClose();
};

END_NAME_SPACE(tGis, Core)


#endif



