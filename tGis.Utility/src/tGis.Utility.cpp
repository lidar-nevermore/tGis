#include "tGis.Utility.h"

#include "tOrganizer.h"
#include "ObjectSampleTool.h"

using namespace tGis::Core;

BEGIN_NAME_SPACE(tGis, Utility)

ToolKit mlToolKit("Machine Learning");
ToolKit tdToolKit("Target Detection");

TGIS_UTILITY_API void InitializeModule()
{
	tdToolKit.AddTool(&(ObjectSampleTool::INSTANCE));
	ToolKitRepository::INSTANCE().AddToolKit(2, &mlToolKit, &tdToolKit);
}


END_NAME_SPACE(tGis, Utility)

