#pragma once

#ifndef __DATASOURCEREPOSITORY_H__
#define __DATASOURCEREPOSITORY_H__

#include "Helper.h"

BEGIN_NAME_SPACE(tGis, Core)

struct IDataSourceProvider;

class TGISCORE_API DataSourceRepository
{
public:
	static const DataSourceRepository INSTANCE;

private:
	DataSourceRepository();
	~DataSourceRepository();
	DataSourceRepository(const DataSourceRepository &) = delete;
	DataSourceRepository &operator=(const DataSourceRepository &) = delete;

public:
	int GetDataSourceProviderCount();
	IDataSourceProvider* GetDataSourceProvider(int);
	void AddDataSourceProvider(char*, IDataSourceProvider*);
	IDataSourceProvider* GetDataSourceProvider(char*);
};

END_NAME_SPACE(tGis, Core)

#endif

