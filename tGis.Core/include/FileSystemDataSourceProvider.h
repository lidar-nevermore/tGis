#pragma once

#include "Helper.h"
#include "IDataSourceProvider.h"
#include <vector>
#include <map>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

struct IDataset;

class TGIS_API FileSystemDataSourceProvider : public IDataSourceProvider
{
	friend class FileSystemDataSource;
public:
	typedef IDataSource*(*CreationUI)(FileSystemDataSourceProvider*);
	typedef void*(*PropertyUI)(FileSystemDataSourceProvider*,IDataSource*,IDataset*);

public:
	static FileSystemDataSourceProvider* _instance;
	static FileSystemDataSourceProvider& INSTANCE();

private:
	FileSystemDataSourceProvider();
	FileSystemDataSourceProvider(const FileSystemDataSourceProvider &) = delete;
	FileSystemDataSourceProvider &operator=(const FileSystemDataSourceProvider &) = delete;

public:
	~FileSystemDataSourceProvider();

private:
	static const char* const _name;
	static const char* const _type;

private:
	CreationUI _uiCreation;
	PropertyUI _uiProperty;
	//该集合中只放CreateDataSource和UI_CreateDataSource创建的FileSystemDataSource
	vector<IDataSource*> _vecDataSource;
	//该集合中放了所有创建出来的FileSystemDataSource
	map<string, IDataSource*> _mapDataSource;


public:
	const char* GetSupportedDataSourceType();
	const char* GetName();
	const char* GetType();

	void SetCreationUI(const CreationUI ui);
	IDataSource* UI_CreateDataSource();
	void SetPropertyUI(const PropertyUI ui);
	void UI_DataSourceProperty(IDataSource*, IDataset*);
	IDataSource* CreateDataSource(const char* path);
	void ReleaseDataSource(IDataSource*);

	int GetDataSourceCount();
	IDataSource* GetDataSource(int);

	void Release();
};

END_NAME_SPACE(tGis, Core)



