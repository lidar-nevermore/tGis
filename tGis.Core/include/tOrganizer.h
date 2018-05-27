#include "tEntity.h"
#include "tVisualize.h"
#include "IDataSourceProvider.h"
#include "ILayerProvider.h"
#include "FileSystemDataSourceProvider.h"
#include "DataSourceProviderRepository.h"
#include "RasterGrayScaleLayerProvider.h"
#include "RasterRgbLayerProvider.h"
#include "VectorSimpleLayerProvider.h"
#include "LayerProviderRepository.h"


BEGIN_NAME_SPACE(tGis, Core)

struct IMapWidget;

TGISCORE_API void SetCurrentMap(IMap*);

TGISCORE_API IMap*  GetCurrentMap();

TGISCORE_API void SetCurrentMapWidget(IMapWidget*);

TGISCORE_API IMapWidget*  GetCurrentMapWidget();

END_NAME_SPACE(tGis, Core)
