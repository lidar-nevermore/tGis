#include "tOrganizer.h"


BEGIN_NAME_SPACE(tGis, Core)

static IMap* g_CurrentMap = nullptr;

static IMapWidget* g_CurrentMapWidget = nullptr;

TGISCORE_API void SetCurrentMap(IMap* map)
{
	g_CurrentMap = map;
}

TGISCORE_API IMap* GetCurrentMap()
{
	return g_CurrentMap;
}

TGISCORE_API void SetCurrentMapWidget(IMapWidget * mapwidget)
{
	g_CurrentMapWidget = mapwidget;
}

TGISCORE_API IMapWidget * GetCurrentMapWidget()
{
	return g_CurrentMapWidget;
}

END_NAME_SPACE(tGis, Core)
