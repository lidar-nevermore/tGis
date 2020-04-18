#pragma once

#ifndef __MYGDALDATASET_H__
#define __MYGDALDATASET_H__

#include "Helper.h"

#include "Dataset.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"

BEGIN_NAME_SPACE(tGis, Core)

struct GDALInitializer;
struct IDataSource;
class MyGDALDatasetImpl;

class TGIS_CORE_API MyGDALDataset : public Dataset
{
	TGIS_DECLARE_NO_COPY_CLASS(MyGDALDataset);

public:
	virtual const char* GetName();

	virtual bool IsReadOnly();	

	virtual const OGRSpatialReference* GetSpatialReference();
	virtual const OGREnvelope* GetEnvelope();

protected:
	MyGDALDataset();
	MyGDALDataset(DataSource* ds, const char* path, GDALAccess eAccess = GA_Update, bool delayOpen = true, bool autoClose = true);

public:
	virtual ~MyGDALDataset();

public:
	GDALDataset* GetGDALDataset();

protected:
	//打开下述构造函数指定得文件
	//MyGDALDataset(DataSource* ds, const char* path, GDALAccess eAccess = GA_Update, bool delayOpen = true, bool autoClose = true);
	void Attach();

	void Attach(const char* file, GDALAccess eAccess, bool autoClose = true);

	void Detach();

	void SetAutoClose(bool autoClose);

	bool GetAutoClose();

protected:
	OGRSpatialReference* _spatialRef;
	OGREnvelope _envelope;
	GDALDataset* _dataset;
	bool _autoClose;
	GDALAccess _eAccess;

private:
	MyGDALDatasetImpl* _impl_;

public:
	//用于在实例化MyGDALDataset之前初始化GDAL
	//如果程序首先就实例化MyGDALDataset，就不需要调用此方法
	static bool GDALInit();

private:
	static GDALInitializer* _GDALInit;

public:
	static size_t GetSupportedRasterFormatCount();
	static const char* GetSupportedRasterFormatName(size_t);
	static bool GetSupportedRasterFormatCreatable(size_t);
	static size_t GetSupportedRasterFormatPos(const char* ext, bool* supported = nullptr);
	static int GetSupportedRasterFormatDriverIndex(size_t);
	static bool IsSupportedRasterFormatExt(const char*);
	static bool IsSupportedRasterFormatFirstExt(const char*);

	static size_t GetSupportedVectorFormatCount();
	static const char* GetSupportedVectorFormatName(size_t);
	static bool GetSupportedVectorFormatCreatable(size_t);
	static size_t GetSupportedVectorFormatPos(const char* ext, bool* supported = nullptr);
	static int GetSupportedVectorFormatDriverIndex(size_t);
	static bool IsSupportedVectorFormatExt(const char*);
	static bool IsSupportedVectorFormatFirstExt(const char*);
};

END_NAME_SPACE(tGis, Core)


#endif



