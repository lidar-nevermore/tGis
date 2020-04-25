#include "DataSourceRepository.h"
#include <memory>
#include <vector>
#include <map>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)


DataSourceRepository* DataSourceRepository::_instance = nullptr;


DataSourceRepository* DataSourceRepository::INSTANCE()
{
	if (_instance == nullptr)
	{
		_instance = new DataSourceRepository();
		static unique_ptr<DataSourceRepository> shit(_instance);
	}

	return _instance;
}

class DataSourceRepositoryImpl
{
public:
	DataSourceRepositoryImpl(DataSourceRepository* owner)
	{
		_owner = owner;
	}

	DataSourceRepository* _owner;

	vector<IDataset*> _vecOpenedDataset;
	vector<IDataSource*> _vecConnectedDataSource;
};


DataSourceRepository::DataSourceRepository()
{
	_impl_ = new DataSourceRepositoryImpl(this);
}


DataSourceRepository::~DataSourceRepository()
{
	delete _impl_;
}


size_t DataSourceRepository::GetOpenedDatasetCount()
{
	return _impl_->_vecOpenedDataset.size();
}

IDataset * DataSourceRepository::GetOpenedDataset(size_t pos)
{
	return _impl_->_vecOpenedDataset.at(pos);
}

size_t DataSourceRepository::GetConnectedDataSourceCount()
{
	return _impl_->_vecConnectedDataSource.size();
}

IDataSource * DataSourceRepository::GetConnectedDataSource(size_t pos)
{
	return _impl_->_vecConnectedDataSource.at(pos);
}

void DataSourceRepository::AddOpenedDataset(IDataset * dt)
{
	_impl_->_vecOpenedDataset.push_back(dt);
	AfterDatasetOpenEvent(dt);
}

void DataSourceRepository::RemoveOpenedDataset(IDataset * dt)
{
	BeforeDatasetCloseEvent(dt);
	for (vector<IDataset*>::iterator it = _impl_->_vecOpenedDataset.begin(); it != _impl_->_vecOpenedDataset.end(); it++)
	{
		if (dt == *it)
		{
			_impl_->_vecOpenedDataset.erase(it);
			break;
		}
	}
}

void DataSourceRepository::AddConnectedDataSource(IDataSource * ds)
{
	_impl_->_vecConnectedDataSource.push_back(ds);
	AfterDataSourceConnectEvent(ds);
}

void DataSourceRepository::RemoveConnectedDataSource(IDataSource * ds)
{
	BeforeDataSourceDisconnectEvent(ds);
	for (vector<IDataSource*>::iterator it = _impl_->_vecConnectedDataSource.begin(); it != _impl_->_vecConnectedDataSource.end(); it++)
	{
		if (ds == *it)
		{
			_impl_->_vecConnectedDataSource.erase(it);
			break;
		}
	}
}


END_NAME_SPACE(tGis, Core)
