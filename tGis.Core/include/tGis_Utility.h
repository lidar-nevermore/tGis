#include "tGis_CoreCfg.h"

#include "tinyxml2.h"

extern "C"
{
#include "elr_mpl.h"
#include "ilog.h"
#include "hashmap.h"
#include "bi_list.h"
#include "sorted_list.h"
#include "linked_array.h"
}

#include "MyGDALRasterDataset.h"
#include "RasterBandSeqBlockReader.h"
#include "RasterBandSeqPixelReader.h"
#include "RasterBandStorageBlockGridReader.h"
#include "Statistics.h"
#include "RasterWriter.h"

#include "Progress.h"