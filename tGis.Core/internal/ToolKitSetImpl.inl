#pragma once

#ifndef __ToolKitSetImpl_INL__
#define __ToolKitSetImpl_INL__

#include "Helper.h"

#include <string>
#include <vector>
#include <map>

using namespace std;


BEGIN_NAME_SPACE(tGis, Core)

class ToolKitSet;
class ToolKit;
class StandaloneTool;

class ToolKitSetImpl
{
public:
	ToolKitSetImpl(ToolKitSet* owner)
	{
		_owner = owner;
	}

	ToolKitSet* _owner;

	vector<StandaloneTool*> _vecStandaloneTool;
	vector<ToolKit*> _vecToolKit;
	map<string, ToolKit*> _mapToolKit;
};

END_NAME_SPACE(tGis, Core)


#endif


