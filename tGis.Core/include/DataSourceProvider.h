#pragma once


#ifndef __DATASOURCEPROVIDER_H__
#define __DATASOURCEPROVIDER_H__


#include "Helper.h"
#include "IDataSourceProvider.h"
#include <vector>
#include <map>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

struct IDataset;

class TGIS_CORE_API DataSourceProvider : public IDataSourceProvider
{
	friend class DataSource;
protected:
	DataSourceProvider();
	DataSourceProvider(const DataSourceProvider &) = delete;
	DataSourceProvider &operator=(const DataSourceProvider &) = delete;

private:
	//直接子类DataSourceProvider的实例集合
	vector<IDataSourceProvider*> _vecSubProvider;

	void AfterDatasetOpenSubProvider(IDataSourceProvider*, IDataset*);
	void BeforeDatasetCloseSubProvider(IDataSourceProvider*, IDataset*);

	MemberEventHandler<DataSourceProvider, IDataSourceProvider*, IDataset*> _AfterDatasetOpenHandler;
	MemberEventHandler<DataSourceProvider, IDataSourceProvider*, IDataset*> _BeforeDatasetCloseHandler;

	void AfterDataSourceConnectSubProvider(IDataSourceProvider*, IDataSource*);
	void BeforeDataSourceDisconnectSubProvider(IDataSourceProvider*, IDataSource*);

public:
	virtual ~DataSourceProvider();

protected:
	//该集合中存放所有打开的数据集，包括子Provider打开的数据集
	vector<IDataset*> _vecOpenedDataset;

	virtual void AddOpenedDataset(IDataset*);
	virtual void RemoveOpenedDataset(IDataset*);

	vector<IDataSource*> _vecConnectedDataSource;

	virtual void AddConnectedDataSource(IDataSource*);
	virtual void RemoveConnectedDataSource(IDataSource*);

public:
	virtual void AddSubProvider(IDataSourceProvider*);

	virtual IDataSource* CreateDataSource(const char * creationString);
	virtual void ReleaseDataSource(IDataSource*);

	virtual size_t GetOpenedDatasetCount();
	virtual IDataset* GetOpenedDataset(size_t);

	virtual size_t GetConnectedDataSourceCount();
	virtual IDataSource* GetConnectedDataSource(size_t);

	virtual void Release();
};



END_NAME_SPACE(tGis, Core)


#endif


