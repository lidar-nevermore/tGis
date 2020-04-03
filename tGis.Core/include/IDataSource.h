#pragma once

#ifndef __I_DATASOURCE_H__
#define __I_DATASOURCE_H__

#include "Helper.h"
#include "ITGisObject.h"

BEGIN_NAME_SPACE(tGis, Core)

struct IDataset;

struct TGIS_CORE_API IDataSource : public ITGisObject
{
	friend class DataSource;

	virtual const char* GetName() = 0;

	virtual bool IsConnected() = 0;
	virtual void Connect() = 0;
	virtual void Disconnect() = 0;
	virtual IDataSource* GetDataSource() = 0;

	virtual size_t GetDatasetCount() = 0;
	virtual IDataset* GetDataset(size_t) = 0;

	virtual size_t GetOpenedDatasetCount() = 0;
	virtual IDataset* GetOpenedDataset(size_t) = 0;

	virtual size_t GetDataSourceCount() = 0;
	virtual IDataSource* GetDataSource(size_t) = 0;

	virtual size_t GetConnectedDataSourceCount() = 0;
	virtual IDataSource* GetConnectedDataSource(size_t) = 0;

	IDataSource() {};
	virtual ~IDataSource() {};
private:
	IDataSource(const IDataSource &) = delete;
	IDataSource &operator=(const IDataSource &) = delete;
};

typedef IDataSource* IDataSourcePtr;

END_NAME_SPACE(tGis, Core)



#endif



