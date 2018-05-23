#pragma once

#ifndef __I_DATASOURCEPROVIDER_H__
#define __I_DATASOURCEPROVIDER_H__


#include "Helper.h"

BEGIN_NAME_SPACE(tGis, Core)

struct IDataSource;

struct TGISCORE_API IDataSourceProvider
{
	virtual const char* GetSupportedDataSourceType() = 0;
	virtual const char* GetCatagory() = 0;
	virtual const char* GetType() = 0;

	virtual IDataSource* UI_CreateDataSource() = 0;
	virtual IDataSource* CreateDataSource(const char*) = 0;
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


END_NAME_SPACE(tGis, Core)


#endif
