#pragma once

#ifndef __DATASOURCEREPOSITORY_H__
#define __DATASOURCEREPOSITORY_H__

#include "Helper.h"
#include "tGisCoreCfg.h"
#include "Event.h"

#include <vector>
#include <map>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

struct IDataSource;
struct IDataset;

template struct TGIS_CORE_API IEventHandler<IDataset*>;
template class TGIS_CORE_API Event<IDataset*>;

template struct TGIS_CORE_API IEventHandler<IDataSource*>;
template class TGIS_CORE_API Event<IDataSource*>;

class TGIS_CORE_API DataSourceRepository
{
	friend class DataSource;
	friend class Dataset;
private:
	static DataSourceRepository* _instance;
public:
	static DataSourceRepository& INSTANCE();

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
	//该集合中存放所有打开的数据集，包括子Provider打开的数据集
	vector<IDataset*> _vecOpenedDataset;

	void AddOpenedDataset(IDataset*);
	void RemoveOpenedDataset(IDataset*);

	vector<IDataSource*> _vecConnectedDataSource;

	void AddConnectedDataSource(IDataSource*);
	void RemoveConnectedDataSource(IDataSource*);
};

END_NAME_SPACE(tGis, Core)

#endif

