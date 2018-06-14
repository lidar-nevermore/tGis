#pragma once

#include "Helper.h"
#include "FileSystemDataSourceProvider.h"
#include <vector>
#include <map>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

class TGIS_API ObjectSampleDataSourceProvider : public FileSystemDataSourceProvider
{
	friend class FileSystemDataSource;
public:
	typedef IDataSource*(*CreationUI)(ObjectSampleDataSourceProvider*);
	typedef void*(*PropertyUI)(ObjectSampleDataSourceProvider*, IDataSource*, IDataset*);

public:
	static ObjectSampleDataSourceProvider* _instance;
	static ObjectSampleDataSourceProvider& INSTANCE();

	static bool IsObjectSampleDataSource(const char* path);

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
	CreationUI _uiCreation;
	PropertyUI _uiProperty;

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
};


END_NAME_SPACE(tGis, Core)


