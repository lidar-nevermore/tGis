#pragma once

#ifndef __DATASOURCE_H__
#define __DATASOURCE_H__

#include "IDataSource.h"

#include "Helper.h"
#include <vector>
#include <map>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)


class TGIS_API DataSource : public IDataSource
{
public:
	DataSource();
	virtual ~DataSource();

protected:
	string _name;
	bool _connected;

	vector<IDataSource*> _vecDataSource;
	map<string, IDataSource*> _mapDataSource;

	vector<IDataset*> _vecDataset;
	map<string, IDataset*> _mapDataset;

public:
	virtual const char* GetName();
	virtual const char* GetCreationString();

	virtual bool IsConnected();
	virtual void Connect();
	virtual void Disconnect();

	virtual int GetDatasetCount();
	virtual IDataset* GetDataset(int);
	virtual IDataset* GetDataset(const char* name);

	virtual int GetDataSourceCount();
	virtual IDataSource* GetDataSource(int);
	virtual IDataSource* GetDataSource(const char* name);
};


END_NAME_SPACE(tGis, Core)

#endif


