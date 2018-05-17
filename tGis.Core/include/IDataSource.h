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
	virtual const char* GetConnectionString() = 0;

	virtual bool IsConnected() = 0;
	virtual void Connect() = 0;
	virtual void Disconnect() = 0;

	virtual int GetDatasetCount() = 0;
	virtual IDataset* GetDataset(int) = 0;
	virtual IDataset* GetDataset(char*) = 0;

	virtual int GetDataSourceCount() = 0;
	virtual IDataSource* GetDataSource(int) = 0;
	virtual IDataSource* GetDataSource(char*) = 0;

	virtual ~IDataSource() {};
};

END_NAME_SPACE(tGis, Core)



#endif



