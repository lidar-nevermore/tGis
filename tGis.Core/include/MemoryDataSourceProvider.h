#pragma once


#ifndef __MEMORYDATASOURCEPROVIDER_H__
#define __MEMORYDATASOURCEPROVIDER_H__


#include "Helper.h"
#include "DataSourceProvider.h"
#include <vector>
#include <map>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

struct IDataset;

class TGIS_API MemoryDataSourceProvider : public DataSourceProvider
{
	friend class MemoryDataSource;
public:
	static MemoryDataSourceProvider* _instance;
	static MemoryDataSourceProvider& INSTANCE();

protected:
	MemoryDataSourceProvider();
	MemoryDataSourceProvider(const MemoryDataSourceProvider &) = delete;
	MemoryDataSourceProvider &operator=(const MemoryDataSourceProvider &) = delete;

public:
	~MemoryDataSourceProvider();

private:
	static const char* const _name;
	static const char* const _type;

public:
	virtual const char* GetSupportedDataSourceType();
	virtual const char* GetName();
	virtual const char* GetType();
	virtual bool IsTypeOf(const char* type);
	virtual bool IsTypeOf(ITGisObject* object);

	virtual IDataSource* CreateDataSource(const char* creationString);
};

END_NAME_SPACE(tGis, Core)

#endif


