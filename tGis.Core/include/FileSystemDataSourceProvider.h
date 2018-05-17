#pragma once

#include "Helper.h"
#include "IDataSourceProvider.h"
#include <vector>
#include <map>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

class TGISCORE_API FileSystemDataSourceProvider : public IDataSourceProvider
{
	friend class FileSystemDataSource;
public:
	typedef char*(*UI)();

public:
    static FileSystemDataSourceProvider INSTANCE;

private:
	FileSystemDataSourceProvider();
	~FileSystemDataSourceProvider();
	FileSystemDataSourceProvider(const FileSystemDataSourceProvider &) = delete;
	FileSystemDataSourceProvider &operator=(const FileSystemDataSourceProvider &) = delete;

private:
	static const char* const _catagory;

private:
	UI _ui;
	vector<IDataSource*> _vecDataSource;
	map<string, IDataSource*> _mapDataSource;


public:
	const char* GetSupportedDataSourceType();
	const char* GetCatagory();

	void SetCreateDataSourceUI(const UI ui);

	IDataSource* UI_CreateDataSource();
	IDataSource* CreateDataSource(const char* path);
	void ReleaseDataSource(IDataSource*);

	int GetDataSourceCount();
	IDataSource* GetDataSource(int);

	void Release();
};

END_NAME_SPACE(tGis, Core)



