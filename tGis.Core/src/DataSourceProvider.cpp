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
	Release();
}

bool DataSourceProvider::IsRoot()
{
	return false;
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
	return nullptr;
}

void DataSourceProvider::ReleaseDataSource(IDataSource *ds)
{
	map<string, IDataSource*>::iterator pos = _mapDataSource.find(ds->GetCreationString());

	if (pos != _mapDataSource.end())
		delete (*pos).second;
}

int DataSourceProvider::GetDataSourceCount()
{
	return _vecDataSource.size();
}

IDataSource * DataSourceProvider::GetDataSource(int pos)
{
	return _vecDataSource.at(pos);
}

void DataSourceProvider::Release()
{
	for (vector<IDataSource*>::iterator it = _vecDataSource.begin(); it != _vecDataSource.end(); it++)
	{
		delete (*it);
	}
	_vecDataSource.clear();
}



END_NAME_SPACE(tGis, Core)


