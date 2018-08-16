#include "DataSource.h"
#include "DataSourceProvider.h"


BEGIN_NAME_SPACE(tGis, Core)


DataSource::DataSource(IDataSourceProvider * provider)
{
	_provider = provider;
}

DataSource::~DataSource()
{
}


void DataSource::AddOpenedDataset(IDataset * dt)
{
	_vecOpenedDataset.push_back(dt);
	((DataSourceProvider*)_provider)->AddOpenedDataset(dt);
}

void DataSource::RemoveOpenedDataset(IDataset * dt)
{
	for (vector<IDataset*>::iterator it = _vecOpenedDataset.begin(); it != _vecOpenedDataset.end(); it++)
	{
		if (dt == *it)
		{
			_vecOpenedDataset.erase(it);
			break;
		}
	}
	((DataSourceProvider*)_provider)->RemoveOpenedDataset(dt);
}

const char * DataSource::GetName()
{
	return _name.c_str();
}

const char * DataSource::GetCreationString()
{
	return nullptr;
}

IDataSourceProvider * DataSource::GetProvider()
{
	return _provider;
}

bool DataSource::IsConnected()
{
	return _connected;
}

void DataSource::Connect()
{
	DataSourceProvider* prd = (DataSourceProvider*)_provider;
	prd->AddConnectedDataSource(this);
}

void DataSource::Disconnect()
{
	DataSourceProvider* prd = (DataSourceProvider*)_provider;
	prd->RemoveConnectedDataSource(this);
}

size_t DataSource::GetDatasetCount()
{
	return _vecDataset.size();
}

IDataset * DataSource::GetDataset(size_t pos)
{
	return _vecDataset.at(pos);
}

size_t DataSource::GetDataSourceCount()
{
	return _vecDataSource.size();
}

IDataSource * DataSource::GetDataSource(size_t pos)
{
	return _vecDataSource.at(pos);
}

size_t DataSource::GetOpenedDatasetCount()
{
	return _vecOpenedDataset.size();
}

IDataset * DataSource::GetOpenedDataset(size_t pos)
{
	return _vecOpenedDataset.at(pos);
}


END_NAME_SPACE(tGis, Core)



