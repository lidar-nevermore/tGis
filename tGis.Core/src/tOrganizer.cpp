#include "tOrganizer.h"


BEGIN_NAME_SPACE(tGis, Core)

IMap* g_CurrentMap = nullptr;

void SetCurrentMap(IMap* map)
{
	g_CurrentMap = map;
}

IMap* GetCurrentMap()
{
	return g_CurrentMap;
}

END_NAME_SPACE(tGis, Core)
