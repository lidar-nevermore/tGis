#pragma once

#ifndef __DATASOURCE_H__
#define __DATASOURCE_H__

#include "IDataSource.h"

#include "Helper.h"
#include <vector>
#include <map>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)


class TGIS_API DataSource : public IDataSource
{
	friend class Dataset;
protected:
	DataSource(IDataSourceProvider*);

public:
	virtual ~DataSource();

protected:
	string _name;
	bool _connected;
	IDataSourceProvider* _provider;

	vector<IDataSource*> _vecDataSource;

	vector<IDataset*> _vecDataset;

	//该集合中存放直接归属本数据源的所有打开的数据集
	vector<IDataset*> _vecOpenedDataset;

	void AddOpenedDataset(IDataset*);
	void RemoveOpenedDataset(IDataset*);

public:
	virtual const char* GetName();
	virtual const char* GetCreationString();
	virtual IDataSourceProvider* GetProvider();

	virtual bool IsConnected();
	virtual void Connect();
	virtual void Disconnect();

	virtual int GetDatasetCount();
	virtual IDataset* GetDataset(int);

	virtual int GetDataSourceCount();
	virtual IDataSource* GetDataSource(int);

	virtual int GetOpenedDatasetCount();
	virtual IDataset* GetOpenedDataset(int);
};


END_NAME_SPACE(tGis, Core)

#endif


