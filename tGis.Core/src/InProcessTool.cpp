#include "InProcessTool.h"


BEGIN_NAME_SPACE(tGis, Core)

const char* const InProcessTool::_type = "5EB170BB-C30D-4097-95C2-2F03206FDAD8";


const char * InProcessTool::GetType()
{
	return _type;
}

bool InProcessTool::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return false;
}

const char * InProcessTool::S_GetType()
{
	return _type;
}

InProcessTool::InProcessTool()
{
}


InProcessTool::~InProcessTool()
{
}


END_NAME_SPACE(tGis, Core)

