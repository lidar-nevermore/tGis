#include "DataSourceProvider.h"
#include "IDataSource.h"
#include <cassert>

using namespace std;
BEGIN_NAME_SPACE(tGis, Core)

DataSourceProvider::DataSourceProvider()
{
}


DataSourceProvider::~DataSourceProvider()
{
}

void DataSourceProvider::AddOpenedDataset(IDataset * dt)
{
	_vecOpenedDataset.push_back(dt);
}

void DataSourceProvider::RemoveOpenedDataset(IDataset * dt)
{
	for (vector<IDataset*>::iterator it = _vecOpenedDataset.begin(); it != _vecOpenedDataset.end(); it++)
	{
		if (dt == *it)
		{
			_vecOpenedDataset.erase(it);
			break;
		}
	}
}

void DataSourceProvider::AddSubProvider(IDataSourceProvider * provider)
{
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

void DataSourceProvider::Release()
{
	delete this;
}



END_NAME_SPACE(tGis, Core)


