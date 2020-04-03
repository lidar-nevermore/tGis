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
private:
	static MemoryDataSource* _instance;
public:
	static MemoryDataSource& INSTANCE();

protected:
	MemoryDataSource();

public:
	~MemoryDataSource();

private:
	static const char* const _type;

public:
	virtual const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);
	virtual bool IsTypeOf(ITGisObject* object);
};

END_NAME_SPACE(tGis, Core)


#endif



