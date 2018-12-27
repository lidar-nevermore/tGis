#include "tGisCoreCfg.h"
#include "tEntity.h"
#include "tVisualize.h"

#include "IDataSourceProvider.h"
#include "ILayerProvider.h"
#include "DataSourceProvider.h"
#include "LayerProvider.h"
#include "DataSourceProviderRepository.h"
#include "LayerProviderRepository.h"

#include "FileSystemDataSourceProvider.h"
#include "ObjectSampleDataSourceProvider.h"
#include "MemoryDataSourceProvider.h"

#include "RasterGrayScaleLayerProvider.h"
#include "RasterBinaryGrayScaleLayerProvider.h"
#include "RasterRgbLayerProvider.h"
#include "VectorSimpleLayerProvider.h"

#include "ToolKitRepository.h"

#include "TGisApplication.h"
