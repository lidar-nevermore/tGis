#pragma once

#ifndef __I_DATASOURCE_H__
#define __I_DATASOURCE_H__

#include "Helper.h"

BEGIN_NAME_SPACE(tGis, Core)

struct IDataset;

struct TGISCORE_API IDataSource
{
	virtual const char* GetType() = 0;
	virtual const char* GetName() = 0;
	virtual const char* GetCreationString() = 0;

	virtual bool IsConnected() = 0;
	virtual void Connect() = 0;
	virtual void Disconnect() = 0;

	virtual int GetDatasetCount() = 0;
	virtual IDataset* GetDataset(int) = 0;
	virtual IDataset* GetDataset(const char*) = 0;

	virtual int GetDataSourceCount() = 0;
	virtual IDataSource* GetDataSource(int) = 0;
	virtual IDataSource* GetDataSource(const char*) = 0;

	IDataSource() {};
	virtual ~IDataSource() {};
private:
	IDataSource(const IDataSource &) = delete;
	IDataSource &operator=(const IDataSource &) = delete;
};

END_NAME_SPACE(tGis, Core)



#endif



