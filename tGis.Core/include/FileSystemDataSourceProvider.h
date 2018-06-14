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
	CreationUI _uiCreation;
	PropertyUI _uiProperty;

protected:
	//�ü�����ֻ��CreateDataSource��UI_CreateDataSource������FileSystemDataSource
	vector<IDataSource*> _vecDataSource;
	//�ü����з������д���������FileSystemDataSource
	map<string, IDataSource*> _mapDataSource;

private:
	IDataSource* CreateDataSourceNoHost(const char* path);

public:
	virtual const char* GetSupportedDataSourceType();
	virtual bool IsRoot();
	virtual const char* GetName();
	virtual const char* GetType();

	void SetCreationUI(const CreationUI ui);
	virtual IDataSource* UI_CreateDataSource();
	void SetPropertyUI(const PropertyUI ui);
	virtual void UI_DataSourceProperty(IDataSource*, IDataset*);
	virtual IDataSource* CreateDataSource(const char* path);
	virtual void ReleaseDataSource(IDataSource*);

	virtual int GetDataSourceCount();
	virtual IDataSource* GetDataSource(int);

	virtual void Release();
};

END_NAME_SPACE(tGis, Core)



