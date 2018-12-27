#include "tGisSampling.h"

#include "tOrganizer.h"
#include "ObjectSampleTool.h"

using namespace tGis::Core;

ToolKit mlToolKit("Sampling");
ToolKit tdToolKit("Object Sampling");

void TGIS_SYMBOL_EXPORT __cdecl _tGisPluginInitialize(void)
{
	tdToolKit.AddTool(&(ObjectSampleTool::INSTANCE));
	ToolKitRepository::INSTANCE().AddToolKit(2, &mlToolKit, &tdToolKit);
}

void TGIS_SYMBOL_EXPORT __cdecl _tGisPluginFinalize(void)
{
	return;
}
