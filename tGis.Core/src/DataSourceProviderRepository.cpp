#include "DataSourceProviderRepository.h"
#include "IDataSourceProvider.h"
#include "FileSystemDataSourceProvider.h"
#include "ObjectSampleDataSourceProvider.h"


BEGIN_NAME_SPACE(tGis, Core)

DataSourceProviderRepository* DataSourceProviderRepository::_instance = nullptr;


DataSourceProviderRepository & DataSourceProviderRepository::INSTANCE()
{
	if (_instance == nullptr)
	{
		_instance = new DataSourceProviderRepository();
		static _tGisObjectDestructor<DataSourceProviderRepository> shit(_instance);
		_instance->AddDataSourceProvider(&FileSystemDataSourceProvider::INSTANCE());
	}

	return *_instance;
}

DataSourceProviderRepository::DataSourceProviderRepository()
{

}


DataSourceProviderRepository::~DataSourceProviderRepository()
{
	for (vector<IDataSourceProvider*>::iterator it = _vecDataSourceProvider.begin(); it != _vecDataSourceProvider.end(); it++)
	{
		IDataSourceProvider* dsp = *it;

		//FileSystemDataSourceProvider放在栈上，程序退出时可能先于DataSourceProviderRepository析构
		//调用已经析构的对象的成员方法是不对的
		//
		//哈哈！但是这个条件判定会导致FileSystemDataSourceProvider一定在DataSourceProviderRepository后析构
		if (dsp != &FileSystemDataSourceProvider::INSTANCE()
			&& dsp != &ObjectSampleDataSourceProvider::INSTANCE())
			dsp->Release();
	}
}

void DataSourceProviderRepository::AfterDatasetOpen(IDataSourceProvider * provider, IDataset * dt)
{
	AfterDatasetOpenEvent(provider, dt);
}

void DataSourceProviderRepository::BeforeDatasetClose(IDataSourceProvider * provider, IDataset * dt)
{
	BeforeDatasetCloseEvent(provider, dt);
}

void DataSourceProviderRepository::AfterDataSourceConnect(IDataSourceProvider * provider, IDataSource * ds)
{
	AfterDataSourceConnectEvent(provider, ds);
}

void DataSourceProviderRepository::BeforeDataSourceDisconnect(IDataSourceProvider *provider, IDataSource * ds)
{
	BeforeDataSourceDisconnectEvent(provider, ds);
}

size_t DataSourceProviderRepository::GetDataSourceProviderCount()
{
	return _vecDataSourceProvider.size();
}

IDataSourceProvider * DataSourceProviderRepository::GetDataSourceProvider(size_t pos)
{
	return _vecDataSourceProvider.at(pos);
}

void DataSourceProviderRepository::AddDataSourceProvider(IDataSourceProvider* dsp)
{
	string dataSourceType = dsp->GetSupportedDataSourceType();
	dsp->AfterDatasetOpenEvent.Add(this, &DataSourceProviderRepository::AfterDatasetOpen);
	dsp->BeforeDatasetCloseEvent.Add(this, &DataSourceProviderRepository::BeforeDatasetClose);
	dsp->AfterDataSourceConnectEvent.Add(this, &DataSourceProviderRepository::AfterDataSourceConnect);
	dsp->BeforeDataSourceDisconnectEvent.Add(this, &DataSourceProviderRepository::BeforeDataSourceDisconnect);
	_vecDataSourceProvider.push_back(dsp);
	_mapDataSourceProvider.insert(map<string, IDataSourceProvider*>::value_type(dataSourceType, dsp));
}

IDataSourceProvider * DataSourceProviderRepository::GetDataSourceProvider(const char* dataSourceType)
{
	map<string, IDataSourceProvider*>::iterator pos = _mapDataSourceProvider.find(dataSourceType);

	if (pos != _mapDataSourceProvider.end())
		return (*pos).second;
	return nullptr;
}

END_NAME_SPACE(tGis, Core)
