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

class TGISCORE_API DataSourceProviderRepository
{
public:
	static const DataSourceProviderRepository INSTANCE;

private:
	DataSourceProviderRepository();
	~DataSourceProviderRepository();
	DataSourceProviderRepository(const DataSourceProviderRepository &) = delete;
	DataSourceProviderRepository &operator=(const DataSourceProviderRepository &) = delete;

public:
	int GetDataSourceProviderCount();
	IDataSourceProvider* GetDataSourceProvider(int);
	void AddDataSourceProvider(const char* catagory, IDataSourceProvider*);
	IDataSourceProvider* GetDataSourceProvider(const char* catagory);

private:
	vector<IDataSourceProvider*> _vecDataSourceProvider;
	map<string, IDataSourceProvider*> _mapDataSourceProvider;
};

END_NAME_SPACE(tGis, Core)

#endif

