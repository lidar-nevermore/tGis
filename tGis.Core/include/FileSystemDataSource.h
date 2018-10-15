#pragma once

#ifndef __FILESYSTEMDATASOURCE_H__
#define __FILESYSTEMDATASOURCE_H__

#include "DataSource.h"

#include "Helper.h"
#include <vector>
#include <map>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)


class TGIS_API FileSystemDataSource : public DataSource
{
	friend class FileSystemDataSourceProvider;
protected:
	FileSystemDataSource(const char* path);
	FileSystemDataSource(const char* path, IDataSourceProvider* provider);

public:
	virtual ~FileSystemDataSource();

private:
	int _refCount;

private:
	static const char* const _type;

protected:
	string _path;

private:
	void Disconnect(bool raiseEvent);

public:
	virtual const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);
	virtual bool IsTypeOf(ITGisObject* object);

	virtual const char* GetCreationString();

	virtual void Connect();
	virtual void Connect(const char* creationString, IDataset** dtOut);
	virtual void Connect(const char* creationString, IDataSource** dsOut);
	virtual void Disconnect();
};


END_NAME_SPACE(tGis, Core)

#endif


