#pragma once


#ifndef __MEMORYDATASOURCEPROVIDER_H__
#define __MEMORYDATASOURCEPROVIDER_H__


#include "Helper.h"
#include "DataSourceProvider.h"
#include "MyGDALMemRasterDataset.h"
#include <vector>
#include <map>
#include <string>

#include "Event.h"

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

struct IDataset;

class TGIS_CORE_API MemoryDataSourceProvider : public DataSourceProvider
{
	friend class MemoryDataSource;
public:
	static MemoryDataSourceProvider* _instance;
	static MemoryDataSourceProvider& INSTANCE();

protected:
	MemoryDataSourceProvider();
	MemoryDataSourceProvider(const MemoryDataSourceProvider &) = delete;
	MemoryDataSourceProvider &operator=(const MemoryDataSourceProvider &) = delete;

public:
	~MemoryDataSourceProvider();

private:
	static const char* const _name;
	static const char* const _type;

public:
	virtual const char* GetSupportedDataSourceType();
	virtual const char* GetName();
	virtual const char* GetType();
	virtual bool IsTypeOf(const char* type);
	virtual bool IsTypeOf(ITGisObject* object);

public:
	MyGDALMemRasterDataset* CreateMemRasterDataset(MyGDALRasterDataset* dataset, int left, int top, int width, int height, int b = -1, int * bands = nullptr, bool addToOpened = false, ProgressEvent* progressEvent = nullptr);
	void ReleaseMemRasterDataset(IDataset* dataset, bool removeFromOpened = false);

};

END_NAME_SPACE(tGis, Core)

#endif


