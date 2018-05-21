#include "VectorSimpleLayer.h"
#include "IGeoSurface.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include "ogr_spatialref.h"
#include "ogrsf_frmts.h"
#include "ogr_geometry.h"

BEGIN_NAME_SPACE(tGis, Core)

const char* const VectorSimpleLayer::_type = "57D45D8B-E28E-4D5F-A588-1A55347A0C68";


VectorSimpleLayer::VectorSimpleLayer()
{
	_surfSpatialRef = nullptr;
	_CT = nullptr;
}

VectorSimpleLayer::VectorSimpleLayer(OGRLayer *layer, int geometryField, int labelField)
	:VectorLayer(layer)
{
	_geometryField = geometryField;
	_labelField = labelField;
	_surfSpatialRef = nullptr;
	_CT = nullptr;
}


VectorSimpleLayer::~VectorSimpleLayer()
{
	if (_CT != nullptr)
	{
		OGRCoordinateTransformation::DestroyCT(_CT);
	}
}


void VectorSimpleLayer::SetOGRLayer(OGRLayer * layer, int geometryField, int labelField)
{
	VectorLayer::SetOGRLayer(layer);
	_geometryField = geometryField;
	_labelField = labelField;
	_surfSpatialRef = nullptr;
}

const char * VectorSimpleLayer::GetType()
{
	return _type;
}

void VectorSimpleLayer::Paint(IGeoSurface * surf)
{
	OGRFeature* feature = nullptr;
	OGRGeometry* geometry = nullptr;
	PrepareCT(surf);
	_layer->ResetReading();
	do
	{
		feature = _layer->GetNextFeature();
		if (feature != nullptr)
		{
			geometry = GetGeometry(feature);
			if (geometry != nullptr)
			{
				OGRwkbGeometryType geometryType = wkbFlatten(geometry->getGeometryType());
				switch (geometryType)
				{
				case wkbPoint:
					DrawPoint(surf, (OGRPoint*)geometry);
					break;
				case wkbLineString:
					DrawLineString(surf, (OGRLineString*)geometry);
					break;
				case wkbPolygon:
					DrawPolygon(surf, (OGRPolygon*)geometry);
					break;
				case wkbMultiPoint:
					DrawMultiPoint(surf, (OGRMultiPoint*)geometry);
					break;
				case wkbMultiLineString:
					DrawMultiLineString(surf, (OGRMultiLineString*)geometry);
					break;
				case wkbMultiPolygon:
					DrawMultiPolygon(surf, (OGRMultiPolygon*)geometry);
					break;
				default:
					break;
				}
			}
		}
	} while (feature != nullptr);
}

inline void VectorSimpleLayer::PrepareCT(IGeoSurface * surf)
{
	const OGRSpatialReference* surfSpatialRef = surf->GetSpatialReference();
	if (_spatialRef != nullptr && surfSpatialRef != nullptr && !(surfSpatialRef->IsSame(_surfSpatialRef)))
	{
		if (_CT != nullptr) 
		{
			OGRCoordinateTransformation::DestroyCT(_CT);
		}

		_CT = OGRCreateCoordinateTransformation(_spatialRef, const_cast<OGRSpatialReference*>(surfSpatialRef));
	}

	_surfSpatialRef = const_cast<OGRSpatialReference*>(surfSpatialRef);
}

inline OGRGeometry * VectorSimpleLayer::GetGeometry(OGRFeature *feature)
{
	if (_geometryField < 0)
		return feature->GetGeometryRef();
	else
		return feature->GetGeomFieldRef(_geometryField);
}

inline const char * VectorSimpleLayer::GetLabel(OGRFeature *)
{
	return nullptr;
}

inline void VectorSimpleLayer::DrawPoint(IGeoSurface*, OGRPoint *)
{
}

inline void VectorSimpleLayer::DrawLineString(IGeoSurface*, OGRLineString *)
{
}

inline void VectorSimpleLayer::DrawPolygon(IGeoSurface* surf, OGRPolygon * geometry)
{
	OGRLinearRing*  exterior = geometry->getExteriorRing();
	if (exterior != nullptr)
	{
		if (_CT != nullptr)
		{
			exterior->transform(_CT);
		}

		int ptcount = exterior->getNumPoints();
		int* x = new int[ptcount];
		int* y = new int[ptcount];
		for (int i = 0; i < ptcount; i++)
		{
			*(x + i) = (int)exterior->getX(i);
			*(y + i) = (int)exterior->getY(i);
			//surf->Spatial2Surface(exterior->getX(i), exterior->getY(i), x + i, y + i);
		}

		//_simpleFillSymbol.Paint(surf, ptcount, x, y, nullptr, nullptr);
		_simpleLineSymbol.Paint(surf, ptcount, x, y, nullptr, nullptr);

		delete[] x;
		delete[] y;
	}
}

inline void VectorSimpleLayer::DrawMultiPoint(IGeoSurface*, OGRMultiPoint *)
{
}

inline void VectorSimpleLayer::DrawMultiLineString(IGeoSurface*, OGRMultiLineString *)
{
}

inline void VectorSimpleLayer::DrawMultiPolygon(IGeoSurface*, OGRMultiPolygon *)
{
}



END_NAME_SPACE(tGis, Core)


