#include "GeoSurface.h"
#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include <algorithm>
#include <cmath>

#include "glad.h"

BEGIN_NAME_SPACE(tGis, Core)

GeoSurface::GeoSurface()
{
}


GeoSurface::~GeoSurface()
{
}

void GeoSurface::BeginPaint(bool isCache)
{
	gladLoadGL();
}


END_NAME_SPACE(tGis, Core)