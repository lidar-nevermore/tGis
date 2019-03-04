#include "tGisWaterExtraction.h"

#include "tUtility.h"
#include "tOrganizer.h"
#include "WaterExtractionTool.h"

using namespace tGis::Core;

ToolKit mlToolKit("Recognition");

void TGIS_SYMBOL_EXPORT __cdecl _tGisPluginInitialize(void)
{
	mlToolKit.AddTool(&(WaterExtractionTool::INSTANCE));
	ToolKitRepository::INSTANCE().AddToolKit(&mlToolKit);
}

void TGIS_SYMBOL_EXPORT __cdecl _tGisPluginFinalize(void)
{
	return;
}
