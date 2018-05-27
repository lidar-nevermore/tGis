#pragma once

#include "IDataSource.h"

#include "Helper.h"
#include <vector>
#include <map>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)


class TGISCORE_API FileSystemDataSource : public IDataSource
{
	friend class FileSystemDataSourceProvider;
public:
	FileSystemDataSource(const char* path);
	~FileSystemDataSource();

private:
	static const char* const _type;

private:
	string _path;
	string _name;
	bool _connected;

	vector<IDataSource*> _vecDataSource;
	map<string, IDataSource*> _mapDataSource;

	vector<IDataset*> _vecDataset;
	map<string, IDataset*> _mapDataset;

public:
	const char* GetType();
	static const char* S_GetType();
	const char* GetName();
	const char* GetCreationString();

	bool IsConnected();
	void Connect();
	void Disconnect();

	int GetDatasetCount();
	IDataset* GetDataset(int);
	IDataset* GetDataset(const char*);

	int GetDataSourceCount();
	IDataSource* GetDataSource(int);
	IDataSource* GetDataSource(const char*);
};


END_NAME_SPACE(tGis, Core)

