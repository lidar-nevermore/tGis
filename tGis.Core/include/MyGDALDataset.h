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

struct GDALInitializer;
struct IDataSource;

class TGIS_CORE_API MyGDALDataset : public Dataset
{
public:
	static bool GDALInit();

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
	static GDALInitializer* _GDALInit;

public:
	static size_t GetSupportedRasterFormatCount();
	static const vector<string>& GetSupportedRasterFormatExt(size_t);
	static const char* GetSupportedRasterFormatName(size_t);
	static bool GetSupportedRasterFormatCreatable(size_t);
	static size_t GetSupportedRasterFormatPos(const char* ext,bool* supported = nullptr);
	static int GetSupportedRasterFormatDriverIndex(size_t);
	static bool IsSupportedRasterFormatExt(const char*);
	static bool IsSupportedRasterFormatFirstExt(const char*);

	static size_t GetSupportedVectorFormatCount();
	static const vector<string>& GetSupportedVectorFormatExt(size_t);
	static const char* GetSupportedVectorFormatName(size_t);
	static bool GetSupportedVectorFormatCreatable(size_t);
	static size_t GetSupportedVectorFormatPos(const char* ext, bool* supported = nullptr);
	static int GetSupportedVectorFormatDriverIndex(size_t);
	static bool IsSupportedVectorFormatExt(const char*);
	static bool IsSupportedVectorFormatFirstExt(const char*);

protected:
	GDALDataset* _dataset;
	bool _autoClose;
	GDALAccess _eAccess;

protected:
	MyGDALDataset(IDataSource* ds);

public:
	MyGDALDataset();
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



