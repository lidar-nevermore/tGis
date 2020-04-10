#pragma once

#ifndef __TGIS_APP_CFG_H__
#define __TGIS_APP_CFG_H__

#include "Helper.h"
#include "tGis_Core.h"

#ifdef TGIS_APP_EXPORTS
  #define TGIS_APP_API TGIS_SYMBOL_EXPORT
#else
  #define TGIS_APP_API TGIS_SYMBOL_IMPORT
#endif


#endif
