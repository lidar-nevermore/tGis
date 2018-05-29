#include "tOrganizer.h"


BEGIN_NAME_SPACE(tGis, Core)

static IMap* g_CurrentMap = nullptr;

static IMapWidget* g_CurrentMapWidget = nullptr;

TGIS_API void SetCurrentMap(IMap* map)
{
	g_CurrentMap = map;
}

TGIS_API IMap* GetCurrentMap()
{
	return g_CurrentMap;
}

TGIS_API void SetCurrentMapWidget(IMapWidget * mapwidget)
{
	g_CurrentMapWidget = mapwidget;
}

TGIS_API IMapWidget * GetCurrentMapWidget()
{
	return g_CurrentMapWidget;
}

END_NAME_SPACE(tGis, Core)
