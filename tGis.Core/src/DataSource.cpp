#include "DataSource.h"


BEGIN_NAME_SPACE(tGis, Core)


DataSource::DataSource()
{
}

DataSource::~DataSource()
{
}

const char * DataSource::GetName()
{
	return _name.c_str();
}

const char * DataSource::GetCreationString()
{
	return nullptr;
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

IDataset * DataSource::GetDataset(const char * name)
{
	map<string, IDataset*>::iterator pos = _mapDataset.find(name);

	if (pos != _mapDataset.end())
		return (*pos).second;
	return nullptr;
}

int DataSource::GetDataSourceCount()
{
	return _vecDataSource.size();
}

IDataSource * DataSource::GetDataSource(int pos)
{
	return _vecDataSource.at(pos);
}

IDataSource * DataSource::GetDataSource(const char * name)
{
	map<string, IDataSource*>::iterator pos = _mapDataSource.find(name);

	if (pos != _mapDataSource.end())
		return (*pos).second;
	return nullptr;
}



END_NAME_SPACE(tGis, Core)



