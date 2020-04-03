#include "DataSourceRepository.h"
#include <memory>


BEGIN_NAME_SPACE(tGis, Core)

DataSourceRepository* DataSourceRepository::_instance = nullptr;


DataSourceRepository & DataSourceRepository::INSTANCE()
{
	if (_instance == nullptr)
	{
		_instance = new DataSourceRepository();
		static unique_ptr<DataSourceRepository> shit(_instance);
	}

	return *_instance;
}

DataSourceRepository::DataSourceRepository()
{

}


DataSourceRepository::~DataSourceRepository()
{

}


size_t DataSourceRepository::GetOpenedDatasetCount()
{
	return _vecOpenedDataset.size();
}

IDataset * DataSourceRepository::GetOpenedDataset(size_t pos)
{
	return _vecOpenedDataset.at(pos);
}

size_t DataSourceRepository::GetConnectedDataSourceCount()
{
	return _vecConnectedDataSource.size();
}

IDataSource * DataSourceRepository::GetConnectedDataSource(size_t pos)
{
	return _vecConnectedDataSource.at(pos);
}

void DataSourceRepository::AddOpenedDataset(IDataset * dt)
{
	_vecOpenedDataset.push_back(dt);
	AfterDatasetOpenEvent(dt);
}

void DataSourceRepository::RemoveOpenedDataset(IDataset * dt)
{
	BeforeDatasetCloseEvent(dt);
	for (vector<IDataset*>::iterator it = _vecOpenedDataset.begin(); it != _vecOpenedDataset.end(); it++)
	{
		if (dt == *it)
		{
			_vecOpenedDataset.erase(it);
			break;
		}
	}
}

void DataSourceRepository::AddConnectedDataSource(IDataSource * ds)
{
	_vecConnectedDataSource.push_back(ds);
	AfterDataSourceConnectEvent(ds);
}

void DataSourceRepository::RemoveConnectedDataSource(IDataSource * ds)
{
	BeforeDataSourceDisconnectEvent(ds);
	for (vector<IDataSource*>::iterator it = _vecConnectedDataSource.begin(); it != _vecConnectedDataSource.end(); it++)
	{
		if (ds == *it)
		{
			_vecConnectedDataSource.erase(it);
			break;
		}
	}
}


END_NAME_SPACE(tGis, Core)
