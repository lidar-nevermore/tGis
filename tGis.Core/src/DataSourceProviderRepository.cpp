#include "DataSourceProviderRepository.h"
#include "IDataSourceProvider.h"

BEGIN_NAME_SPACE(tGis, Core)

DataSourceProviderRepository::DataSourceProviderRepository()
{
}


DataSourceProviderRepository::~DataSourceProviderRepository()
{
	for (vector<IDataSourceProvider*>::iterator it = _vecDataSourceProvider.begin(); it != _vecDataSourceProvider.end(); it++)
	{
		(*it)->Release();
	}
}

int DataSourceProviderRepository::GetDataSourceProviderCount()
{
	return _vecDataSourceProvider.size();
}

IDataSourceProvider * DataSourceProviderRepository::GetDataSourceProvider(int pos)
{
	return _vecDataSourceProvider.at(pos);
}

void DataSourceProviderRepository::AddDataSourceProvider(char* catagory,IDataSourceProvider* dsp)
{
	_vecDataSourceProvider.push_back(dsp);
	_mapDataSourceProvider.insert(map<string, IDataSourceProvider*>::value_type(catagory, dsp));
}

IDataSourceProvider * DataSourceProviderRepository::GetDataSourceProvider(char* catagory)
{
	map<string, IDataSourceProvider*>::iterator pos = _mapDataSourceProvider.find(catagory);

	if (pos != _mapDataSourceProvider.end())
		return (*pos).second;
	return nullptr;
}

END_NAME_SPACE(tGis, Core)
