#pragma once

#include "IDataSource.h"

#include "Helper.h"
#include <vector>
#include <map>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)


class TGIS_API FileSystemDataSource : public IDataSource
{
	friend class FileSystemDataSourceProvider;

protected:
	FileSystemDataSource(const char* path);

public:
	~FileSystemDataSource();

private:
	static const char* const _type;

protected:
	string _path;
	string _name;
	bool _connected;

	vector<IDataSource*> _vecDataSource;
	map<string, IDataSource*> _mapDataSource;

	vector<IDataset*> _vecDataset;
	map<string, IDataset*> _mapDataset;

public:
	virtual const char* GetType();
	static const char* S_GetType();
	virtual const char* GetName();
	virtual const char* GetCreationString();

	virtual bool IsConnected();
	virtual void Connect();
	virtual void Disconnect();

	virtual int GetDatasetCount();
	virtual IDataset* GetDataset(int);
	virtual IDataset* GetDataset(const char*);

	virtual int GetDataSourceCount();
	virtual IDataSource* GetDataSource(int);
	virtual IDataSource* GetDataSource(const char*);
};


END_NAME_SPACE(tGis, Core)

