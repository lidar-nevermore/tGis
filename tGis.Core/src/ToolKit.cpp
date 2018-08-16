#include "ToolKit.h"


BEGIN_NAME_SPACE(tGis, Core)


ToolKit::ToolKit(const char* name)
{
	_name = name;
}


ToolKit::~ToolKit()
{
}

const char * ToolKit::GetName()
{
	return _name.c_str();
}

void ToolKit::AddTool(ITool * tool)
{
	_vecTool.push_back(tool);
}

size_t ToolKit::GetToolCount()
{
	return _vecTool.size();
}

ITool * ToolKit::GetTool(size_t pos)
{
	return _vecTool.at(pos);
}

void ToolKit::AddToolKit(ToolKit * kit)
{
	_vecToolKit.push_back(kit);
	_mapToolKit.insert(map<string, ToolKit*>::value_type(kit->GetName(), kit));
}

size_t ToolKit::GetToolKitCount()
{
	return _vecToolKit.size();
}

ToolKit * ToolKit::GetToolKit(size_t pos)
{
	return _vecToolKit.at(pos);
}

ToolKit * ToolKit::GetToolKit(const char * name)
{
	map<string, ToolKit*>::iterator pos = _mapToolKit.find(name);

	if (pos != _mapToolKit.end())
		return (*pos).second;
	return nullptr;
}


END_NAME_SPACE(tGis, Core)
