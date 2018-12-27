#pragma once

#ifndef __DATASOURCEREPOSITORY_H__
#define __DATASOURCEREPOSITORY_H__

#include "Helper.h"
#include "tGisCoreCfg.h"

#include "IDataSourceProvider.h"

#include <vector>
#include <map>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)


class TGIS_CORE_API DataSourceProviderRepository
{
private:
	static DataSourceProviderRepository* _instance;
public:
	static DataSourceProviderRepository& INSTANCE();

private:
	DataSourceProviderRepository();
	DataSourceProviderRepository(const DataSourceProviderRepository &) = delete;
	DataSourceProviderRepository &operator=(const DataSourceProviderRepository &) = delete;

public:
	~DataSourceProviderRepository();

private:
	void AfterDatasetOpen(IDataSourceProvider*, IDataset*);
	void BeforeDatasetClose(IDataSourceProvider*, IDataset*);
	void AfterDataSourceConnect(IDataSourceProvider*, IDataSource*);
	void BeforeDataSourceDisconnect(IDataSourceProvider*, IDataSource*);

public:
	Event<IDataSourceProvider*, IDataset*> AfterDatasetOpenEvent;
	Event<IDataSourceProvider*, IDataset*> BeforeDatasetCloseEvent;
	Event<IDataSourceProvider*, IDataSource*> AfterDataSourceConnectEvent;
	Event<IDataSourceProvider*, IDataSource*> BeforeDataSourceDisconnectEvent;

public:
	size_t GetDataSourceProviderCount();
	IDataSourceProvider* GetDataSourceProvider(size_t);
	void AddDataSourceProvider(IDataSourceProvider*);
	IDataSourceProvider* GetDataSourceProvider(const char* dataSourceType);

private:
	vector<IDataSourceProvider*> _vecDataSourceProvider;
	map<string, IDataSourceProvider*> _mapDataSourceProvider;
};

END_NAME_SPACE(tGis, Core)

#endif

