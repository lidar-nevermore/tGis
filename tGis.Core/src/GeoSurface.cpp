#include "GeoSurface.h"
#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include <algorithm>
#include <cmath>
#include <memory>

#include "glad.h"
#include "glutess.h"

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