#include "ILayerRender.h"

#include "ogr_spatialref.h"


BEGIN_NAME_SPACE(tGis, Core)

bool ILayerRender::CanTransform(const OGRSpatialReference * from, const OGRSpatialReference * to)
{
	if (from == to)
		return true;

	if (from != nullptr)
	{
		if (to == nullptr)
			return false;
		if (from->IsSame(to))
			return true;

		OGRCoordinateTransformation *poCT = OGRCreateCoordinateTransformation(const_cast<OGRSpatialReference*>(from), const_cast<OGRSpatialReference*>(to));
		if (poCT == nullptr)
			return false;

		double x = 76.3;
		double y = 57.0;
		int ret = poCT->Transform(1, &x, &y);
		OGRCoordinateTransformation::DestroyCT(poCT);

		return ret == TRUE;
	}

	return false;
}

END_NAME_SPACE(tGis, Core)


