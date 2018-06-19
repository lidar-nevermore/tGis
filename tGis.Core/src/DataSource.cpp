#include "DataSource.h"


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
}

void DataSource::Disconnect()
{
}

int DataSource::GetDatasetCount()
{
	return _vecDataset.size();
}

IDataset * DataSource::GetDataset(int pos)
{
	return _vecDataset.at(pos);
}

int DataSource::GetDataSourceCount()
{
	return _vecDataSource.size();
}

IDataSource * DataSource::GetDataSource(int pos)
{
	return _vecDataSource.at(pos);
}

int DataSource::GetOpenedDatasetCount()
{
	return _vecOpenedDataset.size();
}

IDataset * DataSource::GetOpenedDataset(int pos)
{
	return _vecOpenedDataset.at(pos);
}


END_NAME_SPACE(tGis, Core)



