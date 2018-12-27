#include "tVisualize.h"
#include "VisualizeBufferManager.h"


BEGIN_NAME_SPACE(tGis, Core)

TGIS_CORE_API void SetMaxSurfaceSize(int width, int height)
{
	VisualizeBufferManager::INSTANCE().SetMaxSurfaceSize(width, height);
}

END_NAME_SPACE(tGis, Core)

