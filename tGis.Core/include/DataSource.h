#pragma once

#ifndef __DATASOURCE_H__
#define __DATASOURCE_H__

#include "IDataSource.h"

#include "Helper.h"

BEGIN_NAME_SPACE(tGis, Core)

class DataSourceImpl;

class TGIS_CORE_API DataSource : public IDataSource
{
	friend class Dataset;
public:
	DataSource();

public:
	virtual ~DataSource();

public:
	virtual const char* GetName();

	virtual bool IsConnected();
	//��������ConnectҪ���û����Connect���������ȵ���
	virtual void Connect();
	//��������DisconnectҪ���û����Disconnect���������ȵ���
	virtual void Disconnect();
	virtual IDataSource* GetParent();

	virtual void Refresh() {};

	virtual size_t GetDatasetCount();

	virtual size_t GetDataSourceCount();

protected:
	bool _connected;
	IDataSource* _parent;

protected:
	void SetName(const char* name);
	void AddDataset(IDataset*);
	void RemoveDataset(IDataset*);
	void RemoveDataset(const char* name);
	void AddDataSource(IDataSource*);
	void RemoveDataSource(IDataSource*);
	void RemoveDataSource(const char* name);

private:
	DataSourceImpl* _impl_;
};


END_NAME_SPACE(tGis, Core)

#endif


