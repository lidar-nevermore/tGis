#include "tGisRegionMerging.h"

#include "tUtility.h"
#include "tOrganizer.h"
#include "RegionMergingTool.h"

using namespace tGis::Core;

ToolKit mlToolKit("Segmentation");

void TGIS_SYMBOL_EXPORT __cdecl _tGisPluginInitialize(void)
{
	mlToolKit.AddTool(&(RegionMergingTool::INSTANCE));
	ToolKitRepository::INSTANCE().AddToolKit(&mlToolKit);
}

void TGIS_SYMBOL_EXPORT __cdecl _tGisPluginFinalize(void)
{
	return;
}
