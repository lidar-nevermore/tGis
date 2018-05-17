#include "DataSourceRepository.h"

BEGIN_NAME_SPACE(tGis, Core)

DataSourceRepository::DataSourceRepository()
{
}


DataSourceRepository::~DataSourceRepository()
{
}

int DataSourceRepository::GetDataSourceProviderCount()
{
	return 0;
}

IDataSourceProvider * DataSourceRepository::GetDataSourceProvider(int)
{
	return nullptr;
}

void DataSourceRepository::AddDataSourceProvider(char *, IDataSourceProvider *)
{
}

IDataSourceProvider * DataSourceRepository::GetDataSourceProvider(char *)
{
	return nullptr;
}

END_NAME_SPACE(tGis, Core)
