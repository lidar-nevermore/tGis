#include "ToolKit.h"
#include "ITool.h"

#include "ToolKitSetImpl.inl"

#include "StandaloneTool.h"

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

void ToolKit::AddTool(ITool * tool)
{
	tool->_parent = this;
	_impl_->_vecTool.push_back(tool);

	//如果加入的是StandaloneTool，则还加入到其自生及所有祖先的_vecStandaloneTool中
	//为得是保存StandaloneTool信息时免得遍历树
	//删除tool的时候也需要注意维护_vecStandaloneTool
	if (tool->IsTypeOf(StandaloneTool::S_GetType()))
	{
		ToolKitSet* parent = this;
		while (nullptr != parent)
		{
			parent->_impl_->_vecStandaloneTool.push_back(dynamic_cast<StandaloneTool*>(tool));
			parent = parent->GetParent();
		}
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
		ToolKitSet* parent = this;
		while (parent != nullptr)
		{
			for (auto it = parent->_impl_->_vecStandaloneTool.begin(); it != parent->_impl_->_vecStandaloneTool.end(); it++)
			{
				ITool* tool_ = *it;
				if (tool_ == tool)
				{
					parent->_impl_->_vecStandaloneTool.erase(it);
					break;
				}
			}

			parent = parent->GetParent();
		}
	}

	if (found && tool->_is_in_heap)
		delete tool;
}

END_NAME_SPACE(tGis, Core)
