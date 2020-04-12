#pragma once

#ifndef __MEMORYDATASOURCE_H__
#define __MEMORYDATASOURCE_H__


#include "DataSource.h"

#include "Helper.h"

BEGIN_NAME_SPACE(tGis, Core)


class TGIS_CORE_API MemoryDataSource : public DataSource
{
	TGIS_DECLARE_NO_COPY_CLASS(MemoryDataSource);
private:
	static MemoryDataSource* _instance;
public:
	static MemoryDataSource& INSTANCE();

public:
	virtual const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);

private:
	static const char* const _type;

protected:
	MemoryDataSource();

public:
	~MemoryDataSource();
};

END_NAME_SPACE(tGis, Core)


#endif



