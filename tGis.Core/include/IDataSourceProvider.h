#pragma once

#ifndef __I_DATASOURCEPROVIDER_H__
#define __I_DATASOURCEPROVIDER_H__


#include "Helper.h"

BEGIN_NAME_SPACE(tGis, Core)

struct IDataSource;
struct IDataset;

struct TGIS_API IDataSourceProvider
{
	virtual const char* GetType() = 0;
	virtual const char* GetName() = 0;

	virtual const char* GetSupportedDataSourceType() = 0;
	virtual bool IsRoot() = 0;

	virtual IDataSource* UI_CreateDataSource() = 0;
	virtual void UI_DataSourceProperty(IDataSource*,IDataset*) = 0;
	virtual IDataSource* CreateDataSource(const char* creationString) = 0;
	virtual void ReleaseDataSource(IDataSource*) = 0;

	virtual int GetDataSourceCount() = 0;
	virtual IDataSource* GetDataSource(int) = 0;

	virtual void Release() = 0;

	IDataSourceProvider() {};
	virtual ~IDataSourceProvider() {};
private:
	IDataSourceProvider(const IDataSourceProvider &) = delete;
	IDataSourceProvider &operator=(const IDataSourceProvider &) = delete;
};

typedef IDataSourceProvider* IDataSourceProviderPtr;

END_NAME_SPACE(tGis, Core)


#endif
