#include "tGisCoreCfg.h"
#include "IDataset.h"
#include "IDataSource.h"
#include "ILayer.h"
#include "ILayerRender.h"
#include "IMap.h"

#include "Map.h"

#include "DataSource.h"
#include "FileSystemDataSource.h"
#include "MemoryDataSource.h"

#include "MyGDALDataset.h"
#include "MyGDALRasterDataset.h"
#include "MyGDALFileRasterDataset.h"
#include "MyGDALMemRasterDataset.h"
#include "MyGDALVectorDataset.h"

#include "Layer.h"
#include "LayerRender.h"

#include "RasterLayerRender.h"
#include "RasterGrayScaleLayerRender.h"
#include "RasterRgbLayerRender.h"
#include "RasterBinaryGrayScaleLayerRender.h"

#include "ISymbol.h"
#include "SimpleMarkerSymbol.h"
#include "SimpleLineSymbol.h"
#include "SimpleFillSymbol.h"

#include "VectorLayerRender.h"
#include "VectorSimpleLayerRender.h"

