#include "DataSource.h"
#include "DataSourceRepository.h"
#include "IDataset.h"

#include <map>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

struct str_ptr_less
{
	bool operator () (const string* a, const string* b) const
	{
		return a->compare(b->c_str()) < 0;
	}
};

struct c_str_ptr_less
{
	bool operator () (const char* a, const char* b) const
	{
		return std::strcmp(a,b) < 0;
	}
};


class DataSourceImpl
{
public:
	DataSourceImpl(DataSource* owner)
	{
		_owner = owner;
	}

	DataSource* _owner;

	string _name;

	map<const char*, IDataSource*, c_str_ptr_less> _mapDataSource;

	map<const char*, IDataset*, c_str_ptr_less> _mapDataset;
};


DataSource::DataSource()
{
	_impl_ = new DataSourceImpl(this);
	_parent = nullptr;
}

DataSource::~DataSource()
{
	Disconnect();
	delete _impl_;
}


void DataSource::SetName(const char * name)
{
	_impl_->_name = name;
}

void DataSource::AddDataset(IDataset * dt)
{
	_impl_->_mapDataset[dt->GetName()] = dt;
}

void DataSource::RemoveDataset(IDataset * dt)
{
	_impl_->_mapDataset.erase(dt->GetName());
}

void DataSource::RemoveDataset(const char * name)
{
	_impl_->_mapDataset.erase(name);
}

void DataSource::AddDataSource(IDataSource *ds)
{
	_impl_->_mapDataSource[ds->GetName()] = ds;
}

void DataSource::RemoveDataSource(IDataSource *ds)
{
	_impl_->_mapDataSource.erase(ds->GetName());
}

void DataSource::RemoveDataSource(const char * name)
{
	_impl_->_mapDataSource.erase(name);
}

const char * DataSource::GetName()
{
	return _impl_->_name.c_str();
}

bool DataSource::IsConnected()
{
	return _connected;
}

void DataSource::Connect()
{
	if (_connected)
		return;

	DataSourceRepository::INSTANCE().AddConnectedDataSource(this);
}

void DataSource::Disconnect()
{
	if (_connected == false)
		return;

	DataSourceRepository::INSTANCE().RemoveConnectedDataSource(this);

	//释放子数据源和子数据集占据的内存
	for (auto it = _impl_->_mapDataset.begin(); it != _impl_->_mapDataset.end(); it++)
	{
		IDataset* dt = (*it).second;
		dt->Close();
		if(dt->_is_in_heap)
			delete dt;
	}
	_impl_->_mapDataset.clear();
		
	for (auto it = _impl_->_mapDataSource.begin(); it != _impl_->_mapDataSource.end(); it++)
	{
		IDataSource* ds = (*it).second;
		ds->Disconnect();
		if (ds->_is_in_heap)
			delete ds;
	}
	_impl_->_mapDataSource.clear();
}

IDataSource * DataSource::GetParent()
{
	return _parent;
}

size_t DataSource::GetDatasetCount()
{
	return _impl_->_mapDataset.size();
}

size_t DataSource::GetDataSourceCount()
{
	return _impl_->_mapDataSource.size();
}


END_NAME_SPACE(tGis, Core)



