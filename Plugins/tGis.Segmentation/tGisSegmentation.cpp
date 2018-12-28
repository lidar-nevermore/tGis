#include "tGisSegmentation.h"

#include "tOrganizer.h"
#include "SegmentationTool.h"

using namespace tGis::Core;

ToolKit mlToolKit("Segmentation");

void TGIS_SYMBOL_EXPORT __cdecl _tGisPluginInitialize(void)
{
	mlToolKit.AddTool(&(SegmentationTool::INSTANCE));
	ToolKitRepository::INSTANCE().AddToolKit(&mlToolKit);
}

void TGIS_SYMBOL_EXPORT __cdecl _tGisPluginFinalize(void)
{
	return;
}
