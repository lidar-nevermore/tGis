#include "DataSourceProvider.h"
#include "IDataSource.h"
#include <cassert>

using namespace std;
BEGIN_NAME_SPACE(tGis, Core)

DataSourceProvider::DataSourceProvider()
	:_AfterDatasetOpenHandler(this,&DataSourceProvider::AfterDatasetOpenSubProvider)
	,_BeforeDatasetCloseHandler(this,&DataSourceProvider::BeforeDatasetCloseSubProvider)
{
}

void DataSourceProvider::AfterDatasetOpenSubProvider(IDataSourceProvider* dsp, IDataset * dt)
{
	this->AddOpenedDataset(dt);
}

void DataSourceProvider::BeforeDatasetCloseSubProvider(IDataSourceProvider* dsp, IDataset * dt)
{
	this->RemoveOpenedDataset(dt);
}

void DataSourceProvider::AfterDataSourceConnectSubProvider(IDataSourceProvider * dsp, IDataSource * ds)
{
	this->AddConnectedDataSource(ds);
}

void DataSourceProvider::BeforeDataSourceDisconnectSubProvider(IDataSourceProvider * dsp, IDataSource * ds)
{
	this->RemoveConnectedDataSource(ds);
}

DataSourceProvider::~DataSourceProvider()
{
}

void DataSourceProvider::AddOpenedDataset(IDataset * dt)
{	
	_vecOpenedDataset.push_back(dt);
	IDataSourceProvider* dsp = this;
	AfterDatasetOpenEvent(dsp, dt);
}

void DataSourceProvider::RemoveOpenedDataset(IDataset * dt)
{
	IDataSourceProvider* dsp = this;
	BeforeDatasetCloseEvent(dsp, dt);
	for (vector<IDataset*>::iterator it = _vecOpenedDataset.begin(); it != _vecOpenedDataset.end(); it++)
	{
		if (dt == *it)
		{
			_vecOpenedDataset.erase(it);
			break;
		}
	}
}

void DataSourceProvider::AddConnectedDataSource(IDataSource * ds)
{
	_vecConnectedDataSource.push_back(ds);
	IDataSourceProvider* dsp = this;
	AfterDataSourceConnectEvent(dsp, ds);
}

void DataSourceProvider::RemoveConnectedDataSource(IDataSource * ds)
{
	IDataSourceProvider* dsp = this;
	BeforeDataSourceDisconnectEvent(dsp, ds);
	for (vector<IDataSource*>::iterator it = _vecConnectedDataSource.begin(); it != _vecConnectedDataSource.end(); it++)
	{
		if (ds == *it)
		{
			_vecConnectedDataSource.erase(it);
			break;
		}
	}
}

void DataSourceProvider::AddSubProvider(IDataSourceProvider * provider)
{
	provider->AfterDatasetOpenEvent += &_AfterDatasetOpenHandler;
	provider->BeforeDatasetCloseEvent += &_BeforeDatasetCloseHandler;
	provider->AfterDataSourceConnectEvent.Add(this, &DataSourceProvider::AfterDataSourceConnectSubProvider);
	provider->BeforeDataSourceDisconnectEvent.Add(this, &DataSourceProvider::BeforeDataSourceDisconnectSubProvider);

	_vecSubProvider.push_back(provider);
}

void DataSourceProvider::SetCreationUI(const CreationUI ui)
{
	_uiCreation = ui;
}

IDataSource * DataSourceProvider::UI_CreateDataSource()
{
	assert(_uiCreation != nullptr);

	return _uiCreation(this);
}

void DataSourceProvider::SetPropertyUI(const PropertyUI ui)
{
	_uiProperty = ui;
}

void DataSourceProvider::UI_DataSourceProperty(IDataSource *ds, IDataset *dt)
{
	assert(_uiProperty != nullptr);

	_uiProperty(this, ds, dt);
}

IDataSource * DataSourceProvider::CreateDataSource(const char * creationString)
{
	for (vector<IDataSourceProvider*>::iterator it = _vecSubProvider.begin(); it != _vecSubProvider.end(); it++)
	{
		IDataSource* ds = (*it)->CreateDataSource(creationString);
		if (ds != nullptr)
		{
			return ds;
		}
	}
	return nullptr;
}

void DataSourceProvider::ReleaseDataSource(IDataSource *ds)
{
	delete ds;
}

int DataSourceProvider::GetOpenedDatasetCount()
{
	return _vecOpenedDataset.size();
}

IDataset * DataSourceProvider::GetOpenedDataset(int pos)
{
	return _vecOpenedDataset.at(pos);
}

int DataSourceProvider::GetConnectedDataSourceCount()
{
	return _vecConnectedDataSource.size();
}

IDataSource * DataSourceProvider::GetConnectedDataSource(int pos)
{
	return _vecConnectedDataSource.at(pos);
}

void DataSourceProvider::Release()
{
	delete this;
}



END_NAME_SPACE(tGis, Core)


