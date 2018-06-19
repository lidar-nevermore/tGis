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

class TGIS_API DataSourceProvider : public IDataSourceProvider
{
	friend class DataSource;
public:
	typedef IDataSource*(*CreationUI)(IDataSourceProvider*);
	typedef void*(*PropertyUI)(IDataSourceProvider*, IDataSource*, IDataset*);

protected:
	DataSourceProvider();
	DataSourceProvider(const DataSourceProvider &) = delete;
	DataSourceProvider &operator=(const DataSourceProvider &) = delete;

private:
	//直接子类DataSourceProvider的实例集合
	vector<IDataSourceProvider*> _vecSubProvider;

public:
	virtual ~DataSourceProvider();

protected:
	CreationUI _uiCreation;
	PropertyUI _uiProperty;

	//该集合中存放所有打开的数据集
	vector<IDataset*> _vecOpenedDataset;

	void AddOpenedDataset(IDataset*);
	void RemoveOpenedDataset(IDataset*);

public:
	void AddSubProvider(IDataSourceProvider*);
	void SetCreationUI(const CreationUI ui);
	virtual IDataSource* UI_CreateDataSource();
	void SetPropertyUI(const PropertyUI ui);
	virtual void UI_DataSourceProperty(IDataSource*, IDataset*);

	virtual IDataSource* CreateDataSource(const char * creationString);
	virtual void ReleaseDataSource(IDataSource*);

	virtual int GetOpenedDatasetCount();
	virtual IDataset* GetOpenedDataset(int);

	virtual void Release();
};



END_NAME_SPACE(tGis, Core)


#endif


