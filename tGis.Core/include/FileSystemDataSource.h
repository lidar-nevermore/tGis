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


class TGIS_CORE_API FileSystemDataSource : public DataSource
{
public:
	FileSystemDataSource(const char* path);
	virtual ~FileSystemDataSource();

private:
	static const char* const _type;

protected:
	string _path;

private:
	static void OnTraverseDir(void* usr, const char * dir, const char* name, unsigned int attrib);

public:
	virtual const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);

	virtual void Connect();
};


END_NAME_SPACE(tGis, Core)

#endif


