#pragma once

#ifndef __OBJECTSAMPLETOOL_H__
#define __OBJECTSAMPLETOOL_H__

#include "tGisCore.h"

using namespace tGis::Core;


class ObjectSampleToolDialog;

class  ObjectSampleTool : public ITool
{

public:
	static ObjectSampleTool INSTANCE;

private:
	ObjectSampleTool();
	~ObjectSampleTool();

private:
	ObjectSampleToolDialog* _widget;

public:
	const char* GetName();
	void Execute();
};





#endif

