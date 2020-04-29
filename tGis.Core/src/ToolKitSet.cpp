#pragma once

#include "ToolKitSet.h"
#include "ToolKit.h"

#include "ToolKitSetImpl.inl"

#include <vector>
#include <map>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)


ToolKitSet::ToolKitSet()
{
	_impl_ = new ToolKitSetImpl(this);
}


ToolKitSet::~ToolKitSet()
{
	for (auto it = _impl_->_vecToolKit.begin(); it != _impl_->_vecToolKit.end(); it++)
	{
		ToolKit* kit = *it;
		if (kit->_is_in_heap)
			delete kit;
	}

	delete _impl_;
}


void ToolKitSet::AddToolKit(ToolKit * kit)
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

size_t ToolKitSet::GetToolKitCount()
{
	return _impl_->_vecToolKit.size();
}

ToolKit * ToolKitSet::GetToolKit(size_t pos)
{
	return _impl_->_vecToolKit.at(pos);
}

ToolKit * ToolKitSet::GetToolKit(const char * name)
{
	map<string, ToolKit*>::iterator pos = _impl_->_mapToolKit.find(name);

	if (pos != _impl_->_mapToolKit.end())
		return (*pos).second;
	return nullptr;
}

void ToolKitSet::RemoveToolKit(ToolKit * kit)
{
	bool found = false;
	for (auto it = _impl_->_vecToolKit.begin(); it != _impl_->_vecToolKit.end(); it++)
	{
		ToolKit* kit_ = *it;
		if (kit_ == kit)
		{
			found = true;
			_impl_->_vecToolKit.erase(it);
			_impl_->_mapToolKit.erase(kit_->GetName());
			break;
		}
	}

	if (found && kit->_is_in_heap)
		delete kit;
}


END_NAME_SPACE(tGis, Core)


