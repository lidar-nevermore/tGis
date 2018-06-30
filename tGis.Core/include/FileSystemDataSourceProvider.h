#pragma once

#include "Helper.h"
#include "DataSourceProvider.h"
#include <vector>
#include <map>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

struct IDataset;

class TGIS_API FileSystemDataSourceProvider : public DataSourceProvider
{
	friend class FileSystemDataSource;
public:
	static FileSystemDataSourceProvider* _instance;
	static FileSystemDataSourceProvider& INSTANCE();

protected:
	FileSystemDataSourceProvider();
	FileSystemDataSourceProvider(const FileSystemDataSourceProvider &) = delete;
	FileSystemDataSourceProvider &operator=(const FileSystemDataSourceProvider &) = delete;

public:
	~FileSystemDataSourceProvider();

protected:
	//该集合中放了所有创建出来的DataSource
	map<string, IDataSource*> _mapDataSource;

private:
	static const char* const _name;
	static const char* const _type;

public:
	virtual const char* GetSupportedDataSourceType();
	virtual const char* GetName();
	virtual const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);
	virtual bool IsTypeOf(ITGisObject* object);

	virtual IDataSource* CreateDataSource(const char* path);
	virtual void ReleaseDataSource(IDataSource*);
};

END_NAME_SPACE(tGis, Core)



