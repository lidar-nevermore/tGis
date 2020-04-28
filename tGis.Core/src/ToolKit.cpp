#include "ToolKit.h"
#include "ITool.h"

#include "StandaloneTool.h"

#include <vector>
#include <map>

using namespace std;


BEGIN_NAME_SPACE(tGis, Core)


class ToolKitImpl
{
public:
	ToolKitImpl(ToolKit* owner)
	{
		_owner = owner;
	}

	ToolKit* _owner;

	string _name;
	vector<StandaloneTool*> _vecStandaloneTool;
	vector<ToolKit*> _vecToolKit;
	map<string, ToolKit*> _mapToolKit;
	vector<ITool*> _vecTool;
};

ToolKit::ToolKit(const char* name, ToolKit* parent)
{
	_impl_ = new ToolKitImpl(this);
	_impl_->_name = name;
	_parent = parent;
}


ToolKit::~ToolKit()
{
	for (auto it = _impl_->_vecTool.begin(); it != _impl_->_vecTool.end(); it++)
	{
		ITool* tool = *it;
		if (tool->_is_in_heap)
			delete tool;
	}

	for (auto it = _impl_->_vecToolKit.begin(); it != _impl_->_vecToolKit.end(); it++)
	{
		ToolKit* kit = *it;
		if (kit->_is_in_heap)
			delete kit;
	}

	delete _impl_;
}

const char * ToolKit::GetName()
{
	return _impl_->_name.c_str();
}

ToolKit * ToolKit::GetParent()
{
	return _parent;
}

void ToolKit::AddTool(ITool * tool)
{
	_impl_->_vecTool.push_back(tool);
}

size_t ToolKit::GetToolCount()
{
	return _impl_->_vecTool.size();
}

ITool * ToolKit::GetTool(size_t pos)
{
	return _impl_->_vecTool.at(pos);
}

void ToolKit::RemoveTool(ITool * tool)
{
	for (auto it = _impl_->_vecTool.begin(); it != _impl_->_vecTool.end(); it++)
	{
		ITool* tool_ = *it;
		if (tool_ == tool)
		{
			_impl_->_vecTool.erase(it);
			if (tool_->_is_in_heap)
				delete tool_;
			break;
		}
	}
}

void ToolKit::AddToolKit(ToolKit * kit)
{
	ToolKit* toFillKit = GetToolKit(kit->GetName());
	if (toFillKit == NULL)
	{
		_impl_->_vecToolKit.push_back(kit);
		_impl_->_mapToolKit.insert(map<string, ToolKit*>::value_type(kit->GetName(), kit));
	}
	else
	{
		size_t toolCount = kit->GetToolCount();
		for (size_t i = 0; i < toolCount; i++)
		{
			toFillKit->AddTool(kit->GetTool(i));
		}

		size_t toolkitCount = kit->GetToolKitCount();
		for (size_t i = 0; i < toolkitCount; i++)
		{
			toFillKit->AddToolKit(kit->GetToolKit(i));
		}

		if (kit->_is_in_heap)
			delete kit;
	}
}

size_t ToolKit::GetToolKitCount()
{
	return _impl_->_vecToolKit.size();
}

ToolKit * ToolKit::GetToolKit(size_t pos)
{
	return _impl_->_vecToolKit.at(pos);
}

ToolKit * ToolKit::GetToolKit(const char * name)
{
	map<string, ToolKit*>::iterator pos = _impl_->_mapToolKit.find(name);

	if (pos != _impl_->_mapToolKit.end())
		return (*pos).second;
	return nullptr;
}

void ToolKit::RemoveToolKit(ToolKit * kit)
{
	for (auto it = _impl_->_vecToolKit.begin(); it != _impl_->_vecToolKit.end(); it++)
	{
		ToolKit* kit_ = *it;
		if (kit_ == kit)
		{
			_impl_->_vecToolKit.erase(it);
			_impl_->_mapToolKit.erase(kit_->GetName());
			if (kit->_is_in_heap)
				delete kit;
			break;
		}
	}
}


END_NAME_SPACE(tGis, Core)
