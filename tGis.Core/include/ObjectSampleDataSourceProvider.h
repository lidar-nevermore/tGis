#pragma once

#include "Helper.h"
#include "FileSystemDataSourceProvider.h"
#include <vector>
#include <map>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

class TGIS_CORE_API ObjectSampleDataSourceProvider : public FileSystemDataSourceProvider
{
	friend class FileSystemDataSource;
public:
	static ObjectSampleDataSourceProvider* _instance;
	static ObjectSampleDataSourceProvider& INSTANCE();

protected:
	ObjectSampleDataSourceProvider();
	ObjectSampleDataSourceProvider(const ObjectSampleDataSourceProvider &) = delete;
	ObjectSampleDataSourceProvider &operator=(const ObjectSampleDataSourceProvider &) = delete;

public:
	~ObjectSampleDataSourceProvider();

private:
	static const char* const _name;
	static const char* const _type;

private:
	bool IsObjectSampleDataSource(const char* path);

public:
	virtual const char* GetSupportedDataSourceType();
	virtual const char* GetName();
	virtual const char* GetType();
	virtual bool IsTypeOf(const char* type);
	virtual bool IsTypeOf(ITGisObject* object);

	virtual IDataSource* CreateDataSource(const char* path);
};


END_NAME_SPACE(tGis, Core)


