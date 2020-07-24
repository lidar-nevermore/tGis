#pragma once

#ifndef __Rasterize_H__
#define __Rasterize_H__

#include "tGis_CoreCfg.h"

BEGIN_NAME_SPACE(tGis, Core)

typedef struct TGIS_CORE_API __POINT_2I_T
{
	int x;
	int y;
}POINT_2I_T;

typedef void(__stdcall *RASTERIZE_FUNC)(void* user, int x, int y);

TGIS_CORE_API void RasterizeLine(int x0, int y0, int x1, int y1, RASTERIZE_FUNC func, void* ud);

TGIS_CORE_API void RasterizePolyline(POINT_2I_T* polyline, size_t size, RASTERIZE_FUNC func, void* ud);

//注意：有些水平线和点会重复访问
TGIS_CORE_API void RasterizePolygon(POINT_2I_T* polygon, size_t size, RASTERIZE_FUNC func, void* ud);

END_NAME_SPACE(tGis, Core)


#endif
