#pragma once

#ifndef __OBJECTSAMPLETOOL_H__
#define __OBJECTSAMPLETOOL_H__

#include "tUtility.h"


using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Utility)

class ObjectSampleToolDialog;

class TGIS_UTILITY_API ObjectSampleTool : public ITool
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


END_NAME_SPACE(tGis, Utility)


#endif

