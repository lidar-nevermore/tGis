#include "IFillSymbol.h"

BEGIN_NAME_SPACE(tGis, Core)

const char* const IFillSymbol::_type = "9DB665DB-94F4-4F5A-813B-0AF3755B32D6";

const char * IFillSymbol::GetType()
{
	return _type;
}

const char * IFillSymbol::S_GetType()
{
	return _type;
}

bool IFillSymbol::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return false;
}

END_NAME_SPACE(tGis, Core)
