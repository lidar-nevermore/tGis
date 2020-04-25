#include "MemoryDataSource.h"
#include <memory>

BEGIN_NAME_SPACE(tGis, Core)

const char* const MemoryDataSource::_type = "EED1ACE6-EF20-45E2-857F-F21E3C3C1A42";

MemoryDataSource* MemoryDataSource::_instance = nullptr;


MemoryDataSource* MemoryDataSource::INSTANCE()
{
	if (_instance == nullptr)
	{
		_instance = new MemoryDataSource();
		static std::unique_ptr<MemoryDataSource> shit(_instance);
	}

	return _instance;
}

MemoryDataSource::MemoryDataSource()
	:DataSource()
{
	SetName("Memory");
	Connect();
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

bool MemoryDataSource::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return false;
}

END_NAME_SPACE(tGis, Core)



