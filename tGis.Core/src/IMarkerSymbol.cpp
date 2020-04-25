#include "IMarkerSymbol.h"

BEGIN_NAME_SPACE(tGis, Core)

const char* const IMarkerSymbol::_type = "4DC7CBE6-8AB2-4F42-AD6C-1288CDA9B777";

const char * IMarkerSymbol::GetType()
{
	return _type;
}

const char * IMarkerSymbol::S_GetType()
{
	return _type;
}

bool IMarkerSymbol::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return false;
}

END_NAME_SPACE(tGis, Core)
