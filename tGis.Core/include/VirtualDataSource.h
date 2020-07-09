#pragma once

#ifndef __VirtualDataSource_H__
#define __VirtualDataSource_H__

#include "Helper.h"

#include "DataSource.h"


BEGIN_NAME_SPACE(tGis, Core)

class VirtualDataSourceImpl;

class TGIS_CORE_API VirtualDataSource : public DataSource
{
	TGIS_DECLARE_NO_COPY_CLASS(VirtualDataSource);

public:
	VirtualDataSource(const char* name);
	virtual ~VirtualDataSource();

private:
	static const char* const _type;

public:
	virtual const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);

public:
	using DataSource::AddDataset;
	using DataSource::RemoveDataset;

private:
	VirtualDataSourceImpl* _impl_;
};


END_NAME_SPACE(tGis, Core)

#endif


