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

TGIS_API void SetCurrentMap(IMap*);

TGIS_API IMap*  GetCurrentMap();

TGIS_API void SetCurrentMapWidget(IMapWidget*);

TGIS_API IMapWidget*  GetCurrentMapWidget();

END_NAME_SPACE(tGis, Core)
