#include "ToolKit.h"
#include "ITool.h"

#include "StandaloneTool.h"
#include "ToolKitSetImpl.inl"

#include <string>
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
	vector<ITool*> _vecTool;
};

ToolKit::ToolKit(const char* name)
{
	_impl_ = new ToolKitImpl(this);
	_impl_->_name = name;
	_parent = nullptr;
}


ToolKit::~ToolKit()
{
	for (auto it = _impl_->_vecTool.begin(); it != _impl_->_vecTool.end(); it++)
	{
		ITool* tool = *it;
		if (tool->_is_in_heap)
			delete tool;
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
	tool->_parent = this;
	_impl_->_vecTool.push_back(tool);

	if (tool->IsTypeOf(StandaloneTool::S_GetType()))
	{
		ToolKitSet::_impl_->_vecStandaloneTool.push_back(dynamic_cast<StandaloneTool*>(tool));
	}
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
	bool found = false;
	for (auto it = _impl_->_vecTool.begin(); it != _impl_->_vecTool.end(); it++)
	{
		ITool* tool_ = *it;
		if (tool_ == tool)
		{
			found = true;
			_impl_->_vecTool.erase(it);
			break;
		}
	}

	if (found && tool->IsTypeOf(StandaloneTool::S_GetType()))
	{
		for (auto it = ToolKitSet::_impl_->_vecStandaloneTool.begin(); it != ToolKitSet::_impl_->_vecStandaloneTool.end(); it++)
		{
			ITool* tool_ = *it;
			if (tool_ == tool)
			{
				ToolKitSet::_impl_->_vecStandaloneTool.erase(it);
				break;
			}
		}
	}

	if (found && tool->_is_in_heap)
		delete tool;
}

void ToolKit::AddToolKit(ToolKit * kit)
{
	kit->_parent = this;
	ToolKitSet::AddToolKit(kit);
}

END_NAME_SPACE(tGis, Core)
