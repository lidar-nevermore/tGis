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
	//ֱ������DataSourceProvider��ʵ������
	vector<IDataSourceProvider*> _vecSubProvider;

	void AfterDatasetOpen(IDataSourceProvider*, IDataset*);
	void BeforeDatasetClose(IDataSourceProvider*, IDataset*);

	//class DS_DatasetEventHandler : public EventHandler<DataSourceProvider, IDataSourceProvider*, IDataset*>
	//{
	//public:
	//	DS_DatasetEventHandler(DataSourceProvider* receiver, EventHandler::Handler handler,)
	//		:EventHandler(receiver,handler)
	//	{

	//	}
	//};

	EventHandler<DataSourceProvider, IDataSourceProvider*, IDataset*> _AfterDatasetOpenHandler;
	EventHandler<DataSourceProvider, IDataSourceProvider*, IDataset*> _BeforeDatasetCloseHandler;

public:
	virtual ~DataSourceProvider();

protected:
	CreationUI _uiCreation;
	PropertyUI _uiProperty;

	//�ü����д�����д򿪵����ݼ�
	vector<IDataset*> _vecOpenedDataset;

	virtual void AddOpenedDataset(IDataset*);
	virtual void RemoveOpenedDataset(IDataset*);

public:
	virtual void AddSubProvider(IDataSourceProvider*);
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


