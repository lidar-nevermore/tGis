#pragma once

#ifndef __DATASOURCEREPOSITORY_H__
#define __DATASOURCEREPOSITORY_H__

#include "Helper.h"
#include "Event.h"


BEGIN_NAME_SPACE(tGis, Core)

class DataSourceRepositoryImpl;
struct IDataSource;
struct IDataset;

template struct TGIS_CORE_API IEventHandler<IDataset*>;
template class TGIS_CORE_API Event<IDataset*>;

template struct TGIS_CORE_API IEventHandler<IDataSource*>;
template class TGIS_CORE_API Event<IDataSource*>;

template struct TGIS_CORE_API IEventHandler<IDataSource*, IDataset*>;
template class TGIS_CORE_API Event<IDataSource*, IDataset*>;

class TGIS_CORE_API DataSourceRepository
{
	friend class DataSource;
	friend class Dataset;
private:
	static DataSourceRepository* _instance;
public:
	static DataSourceRepository* INSTANCE();

private:
	DataSourceRepository();
	DataSourceRepository(const DataSourceRepository &) = delete;
	DataSourceRepository &operator=(const DataSourceRepository &) = delete;

public:
	~DataSourceRepository();

public:
	Event<IDataset*> AfterDatasetOpenEvent;
	Event<IDataset*> BeforeDatasetCloseEvent;
	Event<IDataSource*> AfterDataSourceConnectEvent;
	Event<IDataSource*> BeforeDataSourceDisconnectEvent;

public:
	size_t GetOpenedDatasetCount();
	IDataset* GetOpenedDataset(size_t);

	size_t GetConnectedDataSourceCount();
	IDataSource* GetConnectedDataSource(size_t);

protected:
	void AddOpenedDataset(IDataset*);
	void RemoveOpenedDataset(IDataset*);

	void AddConnectedDataSource(IDataSource*);
	void RemoveConnectedDataSource(IDataSource*);

private:
	DataSourceRepositoryImpl* _impl_;
};

END_NAME_SPACE(tGis, Core)

#endif

