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
public:
	typedef IDataSource*(*CreationUI)(IDataSourceProvider*);
	typedef void*(*PropertyUI)(IDataSourceProvider*, IDataSource*, IDataset*);

protected:
	DataSourceProvider();
	DataSourceProvider(const DataSourceProvider &) = delete;
	DataSourceProvider &operator=(const DataSourceProvider &) = delete;

public:
	virtual ~DataSourceProvider();

protected:
	CreationUI _uiCreation;
	PropertyUI _uiProperty;

	//�ü�����ֻ��CreateDataSource��UI_CreateDataSource������FileSystemDataSource
	vector<IDataSource*> _vecDataSource;
	//�ü����з������д���������FileSystemDataSource
	map<string, IDataSource*> _mapDataSource;

public:
	virtual bool IsRoot();
	void SetCreationUI(const CreationUI ui);
	virtual IDataSource* UI_CreateDataSource();
	void SetPropertyUI(const PropertyUI ui);
	virtual void UI_DataSourceProperty(IDataSource*, IDataset*);

	virtual IDataSource* CreateDataSource(const char * creationString);
	virtual void ReleaseDataSource(IDataSource*);

	virtual int GetDataSourceCount();
	virtual IDataSource* GetDataSource(int);

	virtual void Release();
};



END_NAME_SPACE(tGis, Core)


#endif


