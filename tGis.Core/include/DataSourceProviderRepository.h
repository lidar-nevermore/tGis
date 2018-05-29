#pragma once

#ifndef __DATASOURCEREPOSITORY_H__
#define __DATASOURCEREPOSITORY_H__

#include "Helper.h"

#include <vector>
#include <map>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

struct IDataSourceProvider;

class TGIS_API DataSourceProviderRepository
{
public:
	static DataSourceProviderRepository* _instance;
	static DataSourceProviderRepository& INSTANCE();

private:
	DataSourceProviderRepository();
	DataSourceProviderRepository(const DataSourceProviderRepository &) = delete;
	DataSourceProviderRepository &operator=(const DataSourceProviderRepository &) = delete;

public:
	~DataSourceProviderRepository();

public:
	int GetDataSourceProviderCount();
	IDataSourceProvider* GetDataSourceProvider(int);
	void AddDataSourceProvider(IDataSourceProvider*);
	IDataSourceProvider* GetDataSourceProvider(const char* dataSourceType);

private:
	vector<IDataSourceProvider*> _vecDataSourceProvider;
	map<string, IDataSourceProvider*> _mapDataSourceProvider;
};

END_NAME_SPACE(tGis, Core)

#endif

