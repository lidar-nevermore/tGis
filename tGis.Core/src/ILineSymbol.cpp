#include "ILineSymbol.h"

BEGIN_NAME_SPACE(tGis, Core)

const char* const ILineSymbol::_type = "F1BD8E21-E663-4A78-90A4-22C82162F587";

const char * ILineSymbol::GetType()
{
	return _type;
}

const char * ILineSymbol::S_GetType()
{
	return _type;
}

bool ILineSymbol::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return false;
}

END_NAME_SPACE(tGis, Core)
