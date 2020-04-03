#pragma once

#ifndef __DATASOURCE_H__
#define __DATASOURCE_H__

#include "IDataSource.h"
#include "IDataset.h"

#include "Helper.h"
#include <vector>
#include <map>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)


class TGIS_CORE_API DataSource : public IDataSource
{
	friend class Dataset;
public:
	DataSource();

public:
	virtual ~DataSource();

protected:
	string _name;
	bool _connected;
	IDataSource* _dataSource;

	vector<IDataSource*> _vecDataSource;

	vector<IDataset*> _vecDataset;

	//该集合中存放直接归属本数据源的所有打开的数据集
	vector<IDataset*> _vecOpenedDataset;

	vector<IDataSource*> _vecConnectedDataSource;

	void AddOpenedDataset(IDataset*);
	void RemoveOpenedDataset(IDataset*);

public:
	virtual const char* GetName();

	virtual bool IsConnected();
	virtual void Connect();
	virtual void Disconnect();
	virtual IDataSource* GetDataSource();

	virtual size_t GetDatasetCount();
	virtual IDataset* GetDataset(size_t);

	virtual size_t GetDataSourceCount();
	virtual IDataSource* GetDataSource(size_t);

	virtual size_t GetOpenedDatasetCount();
	virtual IDataset* GetOpenedDataset(size_t);

	virtual size_t GetConnectedDataSourceCount();
	virtual IDataSource* GetConnectedDataSource(size_t);
};


END_NAME_SPACE(tGis, Core)

#endif


