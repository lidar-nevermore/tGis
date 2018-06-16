#include "MemoryDataSourceProvider.h"

#include <cassert>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)


const char* const MemoryDataSourceProvider::_name = "Memory";
const char* const MemoryDataSourceProvider::_type = "5EE4C82D-7773-4BA9-BF97-D5DD2EF4DFAD";

MemoryDataSourceProvider* MemoryDataSourceProvider::_instance = nullptr;

static PtrDestructor<MemoryDataSourceProvider> shit(MemoryDataSourceProvider::_instance);


MemoryDataSourceProvider & MemoryDataSourceProvider::INSTANCE()
{
	if (_instance == nullptr)
	{
		_instance = new MemoryDataSourceProvider();
	}

	return *_instance;
}

MemoryDataSourceProvider::MemoryDataSourceProvider()
{
}


MemoryDataSourceProvider::~MemoryDataSourceProvider()
{
}

const char * MemoryDataSourceProvider::GetSupportedDataSourceType()
{
	return nullptr;
}

bool MemoryDataSourceProvider::IsRoot()
{
	return true;
}

const char * MemoryDataSourceProvider::GetName()
{
	return _name;
}

const char * MemoryDataSourceProvider::GetType()
{
	return _type;
}


IDataSource * MemoryDataSourceProvider::CreateDataSource(const char * creationString)
{
	return nullptr;
}

END_NAME_SPACE(tGis, Core)


