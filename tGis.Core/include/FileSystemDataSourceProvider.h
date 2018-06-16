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

private:
	static const char* const _name;
	static const char* const _type;

private:
	IDataSource* CreateDataSourceNoHost(const char* path);

public:
	virtual const char* GetSupportedDataSourceType();
	virtual bool IsRoot();
	virtual const char* GetName();
	virtual const char* GetType();

	virtual IDataSource* CreateDataSource(const char* path);
};

END_NAME_SPACE(tGis, Core)



