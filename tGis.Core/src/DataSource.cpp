#include "DataSource.h"
#include "DataSourceRepository.h"

BEGIN_NAME_SPACE(tGis, Core)


DataSource::DataSource()
{
	_dataSource = nullptr;
}

DataSource::~DataSource()
{
	Disconnect();
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

bool DataSource::IsConnected()
{
	return _connected;
}

void DataSource::Connect()
{
	if (_connected)
		return;

	_vecConnectedDataSource.push_back(this);
	DataSourceRepository::INSTANCE().AddConnectedDataSource(this);
}

void DataSource::Disconnect()
{
	if (_connected == false)
		return;

	DataSourceRepository::INSTANCE().RemoveConnectedDataSource(this);
	//从父数据源中删除自己
	if (this->_dataSource != nullptr)
	{
		DataSource* parent = (DataSource*)this->_dataSource;
		for (vector<IDataSource*>::iterator it = parent->_vecConnectedDataSource.begin(); it != parent->_vecConnectedDataSource.end(); it++)
		{
			if (this == *it)
			{
				parent->_vecConnectedDataSource.erase(it);
				break;
			}
		}
	}
	//关闭已经打开的子数据源和子数据集
	for (vector<IDataSource*>::iterator it = _vecConnectedDataSource.begin(); it != _vecConnectedDataSource.end(); it++)
		(*it)->Disconnect();
	for (vector<IDataset*>::iterator it = _vecOpenedDataset.begin(); it != _vecOpenedDataset.end(); it++)
		(*it)->Close();
	_vecConnectedDataSource.clear();
	_vecOpenedDataset.clear();

	//释放子数据源和子数据集占据的内存
	for (vector<IDataset*>::iterator it = _vecDataset.begin(); it != _vecDataset.end(); it++)
	{
		IDataset* dt = (IDataset*)(*it);
		if(dt->_is_in_heap)
			delete dt;
	}
		
	for (vector<IDataSource*>::iterator it = _vecDataSource.begin(); it != _vecDataSource.end(); it++)
	{
		IDataSource* ds = (IDataSource*)(*it);
		if (ds->_is_in_heap)
			delete ds;
	}
}

IDataSource * DataSource::GetDataSource()
{
	return _dataSource;
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

size_t DataSource::GetConnectedDataSourceCount()
{
	return _vecConnectedDataSource.size();
}

IDataSource * DataSource::GetConnectedDataSource(size_t pos)
{
	return _vecConnectedDataSource.at(pos);
}


END_NAME_SPACE(tGis, Core)



