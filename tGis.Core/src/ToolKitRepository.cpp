#include "ToolKitRepository.h"
#include "ToolKit.h"

BEGIN_NAME_SPACE(tGis, Core)

ToolKitRepository* ToolKitRepository::_instance = nullptr;

static PtrDestructor<ToolKitRepository> shit(ToolKitRepository::_instance);


ToolKitRepository & ToolKitRepository::INSTANCE()
{
	if (_instance == nullptr)
	{
		_instance = new ToolKitRepository();
	}

	return *_instance;
}

ToolKitRepository::ToolKitRepository()
{
}


ToolKitRepository::~ToolKitRepository()
{
}

void ToolKitRepository::AddToolKit(ToolKit * kit)
{
	_vecToolKit.push_back(kit);
	_mapToolKit.insert(map<string, ToolKit*>::value_type(kit->GetName(), kit));
}

int ToolKitRepository::GetToolKitCount()
{
	return _vecToolKit.size();
}

ToolKit * ToolKitRepository::GetToolKit(int pos)
{
	return _vecToolKit.at(pos);
}

ToolKit * ToolKitRepository::GetToolKit(const char * name)
{
	map<string, ToolKit*>::iterator pos = _mapToolKit.find(name);

	if (pos != _mapToolKit.end())
		return (*pos).second;
	return nullptr;
}


END_NAME_SPACE(tGis, Core)

