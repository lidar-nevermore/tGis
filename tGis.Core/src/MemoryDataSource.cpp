#include "MemoryDataSource.h"
#include "MemoryDataSourceProvider.h"

BEGIN_NAME_SPACE(tGis, Core)

const char* const MemoryDataSource::_type = "EED1ACE6-EF20-45E2-857F-F21E3C3C1A42";

MemoryDataSource::MemoryDataSource(const char* name)
	:DataSource(&MemoryDataSourceProvider::INSTANCE())
{
	_name = name;
}

MemoryDataSource::~MemoryDataSource()
{
}

const char * MemoryDataSource::GetType()
{
	return _type;
}

const char * MemoryDataSource::S_GetType()
{
	return _type;
}

END_NAME_SPACE(tGis, Core)



