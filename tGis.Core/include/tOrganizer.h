#include "tEntity.h"
#include "IDataSourceProvider.h"
#include "ILayerProvider.h"
#include "FileSystemDataSourceProvider.h"
#include "DataSourceRepository.h"

BEGIN_NAME_SPACE(tGis, Core)

TGISCORE_API void SetCurrentMap(IMap*);

TGISCORE_API IMap*  GetCurrentMap();

END_NAME_SPACE(tGis, Core)
