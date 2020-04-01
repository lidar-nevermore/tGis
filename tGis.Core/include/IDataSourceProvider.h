#pragma once

#ifndef __I_DATASOURCEPROVIDER_H__
#define __I_DATASOURCEPROVIDER_H__

#include "Helper.h"
#include "ITGisObject.h"
#include "tGisCoreCfg.h"
#include "Event.h"


BEGIN_NAME_SPACE(tGis, Core)

struct IDataSourceProvider;
struct IDataSource;
struct IDataset;

typedef IEventHandler<IDataSourceProvider*, IDataset*> DatasetEventHandler;

template struct TGIS_CORE_API IEventHandler<IDataSourceProvider*, IDataset*>;
template class TGIS_CORE_API Event<IDataSourceProvider*, IDataset*>;

template struct TGIS_CORE_API IEventHandler<IDataSourceProvider*, IDataSource*>;
template class TGIS_CORE_API Event<IDataSourceProvider*, IDataSource*>;

struct TGIS_CORE_API IDataSourceProvider : public ITGisObject
{
	virtual const char* GetName() = 0;

	virtual const char* GetSupportedDataSourceType() = 0;

	virtual void AddSubProvider(IDataSourceProvider*) = 0;

	virtual IDataSource* CreateDataSource(const char* creationString) = 0;
	virtual void ReleaseDataSource(IDataSource*) = 0;

	virtual size_t GetOpenedDatasetCount() = 0;
	virtual IDataset* GetOpenedDataset(size_t) = 0;

	virtual size_t GetConnectedDataSourceCount() = 0;
	virtual IDataSource* GetConnectedDataSource(size_t) = 0;

	Event<IDataSourceProvider*, IDataset*> AfterDatasetOpenEvent;

	Event<IDataSourceProvider*, IDataset*> BeforeDatasetCloseEvent;

	Event<IDataSourceProvider*, IDataSource*> AfterDataSourceConnectEvent;

	Event<IDataSourceProvider*, IDataSource*> BeforeDataSourceDisconnectEvent;

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
