#pragma once

#ifndef __FILESYSTEMDATASOURCE_H__
#define __FILESYSTEMDATASOURCE_H__

#include "Helper.h"

#include "DataSource.h"


BEGIN_NAME_SPACE(tGis, Core)

class FileSystemDataSourceImpl;

class TGIS_CORE_API FileSystemDataSource : public DataSource
{
	TGIS_DECLARE_NO_COPY_CLASS(FileSystemDataSource);

public:
	FileSystemDataSource(const char* path);
	virtual ~FileSystemDataSource();

private:
	static const char* const _type;

public:
	virtual const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);

public:
	virtual void Connect();

	virtual void Refresh();

private:
	static void OnTraverseDirConnect(void* usr, const char * dir, const char* name, unsigned int attrib);
	static void OnTraverseDirRefresh(void* usr, const char * dir, const char* name, unsigned int attrib);

protected:
	const char* GetPath();

private:
	FileSystemDataSourceImpl* _impl_;
};


END_NAME_SPACE(tGis, Core)

#endif


