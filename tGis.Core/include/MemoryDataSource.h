#pragma once

#ifndef __MEMORYDATASOURCE_H__
#define __MEMORYDATASOURCE_H__


#include "DataSource.h"

#include "Helper.h"
#include <vector>
#include <map>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)


class TGIS_CORE_API MemoryDataSource : public DataSource
{
	friend class MemoryDataSourceProvider;
protected:
	MemoryDataSource(const char* name);

public:
	~MemoryDataSource();

private:
	static const char* const _type;

public:
	virtual const char* GetType();
	static const char* S_GetType();

	void Connect(const char* creationString, IDataset** dtOut);
	void Connect(const char* creationString, IDataSource** dsOut);
};

END_NAME_SPACE(tGis, Core)


#endif



