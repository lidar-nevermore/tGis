#ifndef __TGIS_CORE_CFG_H__
#define __TGIS_CORE_CFG_H__

#include "Helper.h"

#ifdef TGIS_CORE_EXPORTS
  #define TGIS_CORE_API TGIS_SYMBOL_EXPORT
#else
  #define TGIS_CORE_API TGIS_SYMBOL_IMPORT
#endif

#endif
