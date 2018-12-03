#include "VectorSimpleLayer.h"
#include "IGeoSurface.h"
#include "VisualizeBufferManager.h"
#include "MyGDALVectorDataset.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include "ogr_spatialref.h"
#include "ogrsf_frmts.h"
#include "ogr_geometry.h"

BEGIN_NAME_SPACE(tGis, Core)

const char* const VectorSimpleLayer::_type = "57D45D8B-E28E-4D5F-A588-1A55347A0C68";


VectorSimpleLayer::VectorSimpleLayer(ILayerProvider* provider)
	:VectorLayer(provider)
{
}

VectorSimpleLayer::VectorSimpleLayer(ILayerProvider* provider, MyGDALVectorDataset* vector, OGRLayer *layer, int geometryField, int labelField)
	:VectorLayer(provider, vector,layer)
{
	_geometryField = geometryField;
	_labelField = labelField;
}


VectorSimpleLayer::~VectorSimpleLayer()
{
}


void VectorSimpleLayer::SetOGRLayer(MyGDALVectorDataset* vector, OGRLayer * layer, int geometryField, int labelField)
{
	VectorLayer::SetOGRLayer(vector,layer);
	_geometryField = geometryField;
	_labelField = labelField;
	_mapSpatialRef = nullptr;
}

const char * VectorSimpleLayer::GetType()
{
	return _type;
}

const char * VectorSimpleLayer::S_GetType()
{
	return _type;
}

const char * VectorSimpleLayer::GetCreationString()
{
	return nullptr;
}

ILayer * VectorSimpleLayer::Clone(IDataset *dt)
{
	VectorSimpleLayer* layer = new VectorSimpleLayer(_provider);
	memcpy(layer, this, sizeof(VectorSimpleLayer));
	return layer;
}

void VectorSimpleLayer::Paint(IGeoSurface * surf)
{
	if (_visible == false)
		return;

	OGRFeature* feature = nullptr;
	OGRGeometry* geometry = nullptr;
	unsigned char* surfBuffer = VisualizeBufferManager::INSTANCE().AllocSurfaceBuffer();
	unsigned char* pixBuffer = VisualizeBufferManager::INSTANCE().AllocDatasetBuffer(4);
	int* x = (int*)surfBuffer;
	int* y = (int*)pixBuffer;
	PrepareCT();
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
					DrawPoint(x, y, surf, (OGRPoint*)geometry);
					break;
				case wkbLineString:
					DrawLineString(x, y, surf, (OGRLineString*)geometry);
					break;
				case wkbPolygon:
					DrawPolygon(x, y, surf, (OGRPolygon*)geometry);
					break;
				case wkbMultiPoint:
					DrawMultiPoint(x, y, surf, (OGRMultiPoint*)geometry);
					break;
				case wkbMultiLineString:
					DrawMultiLineString(x, y, surf, (OGRMultiLineString*)geometry);
					break;
				case wkbMultiPolygon:
					DrawMultiPolygon(x, y, surf, (OGRMultiPolygon*)geometry);
					break;
				default:
					break;
				}
			}
		}
	} while (feature != nullptr);

	VisualizeBufferManager::INSTANCE().FreeSurfaceBuffer(surfBuffer);
	VisualizeBufferManager::INSTANCE().FreeDatasetBuffer(pixBuffer);
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

inline void VectorSimpleLayer::DrawPoint(int* xb, int* yb, IGeoSurface* surf, OGRPoint *geometry)
{
	if (_CT != nullptr)
	{
		geometry->transform(_CT);
	}
	int x = -1;
	int y = -1;
	double spatialX = geometry->getX();
	double spatialY = geometry->getY();
	OGREnvelope ev;
	ev.MaxX = spatialX;
	ev.MinX = spatialX;
	ev.MaxY = spatialY;
	ev.MinY = spatialY;
	if (_envelope.Contains(ev))
	{
		surf->GetViewPort()->Spatial2Surface(geometry->getX(), geometry->getY(), &x, &y);
		_simpleMarkerSymbol.Paint(surf, 1, &x, &y, nullptr, nullptr);
	}
}

inline int VectorSimpleLayer::TransferGeometryPoints(int * x, int * y, IGeoSurface* surf, OGRSimpleCurve *geometry)
{
	if (_CT != nullptr)
	{
		geometry->transform(_CT);
	}

	int surfW;
	int surfH;
	surf->GetViewPort()->GetSurfaceSize(&surfW, &surfH);
	int ptcount = geometry->getNumPoints();
	int pti = 0;
	int spti = 0;

	int presx;
	int presy;
	int omitedPointCount = 0; //从上一次保留点到现在排除的点数目
	int pointArea; //当前保留点所在的区域；显示区域+以外左上右下分别是012345678
	int keepPointArea[9];//下一次所需保留点出现的区域，数组位置表示区域，其中值为1表示保留

	memset(keepPointArea, 1, 9 * sizeof(int));

	while (pti < ptcount)
	{
		int sx;
		int sy;
		surf->GetViewPort()->Spatial2Surface(geometry->getX(pti), geometry->getY(pti), &sx, &sy);

		*(x + spti) = sx;
		*(y + spti) = sy;

		int curPointArea;
		if (sx >= 0 && sx < surfW && sy >= 0 && sy < surfH)
		{
			curPointArea = 0;
		}
		else if (sx < 0 && sy >= 0 && sy < surfH)
		{
			curPointArea = 1;
		}
		else if (sx < 0 && sy < 0)
		{
			curPointArea = 2;
		}
		else if (sy < 0 && sx >= 0 && sx < surfW)
		{
			curPointArea = 3;
		}
		else if (sy < 0 && sx >= surfW)
		{
			curPointArea = 4;
		}
		else if (sx >= surfW && sy >= 0 && sy < surfH)
		{
			curPointArea = 5;
		}
		else if (sx >= surfW && sy >= surfH)
		{
			curPointArea = 6;
		}
		else if (sy >= surfH && sx >= 0 && sx < surfW)
		{
			curPointArea = 7;
		}
		else if (sx < 0 && sy >= surfH)
		{
			curPointArea = 8;
		}

		if (keepPointArea[curPointArea] != 0 )
		{
			if (pti == 0 || (presx != sx || presy != sy))
			{
				if (omitedPointCount > 0)
				{
					*(x + spti) = presx;
					*(y + spti) = presy;
					spti++;
					*(x + spti) = sx;
					*(y + spti) = sy;
				}
				spti++;
			}
			omitedPointCount = 0;
		}
		else
		{
			omitedPointCount++;
		}
		pti++;

		presx = sx;
		presy = sy;
		memset(keepPointArea, 1, 9 * sizeof(int));
		if(curPointArea != 0)
			keepPointArea[curPointArea] = 0;
		pointArea = curPointArea;

		//switch (pointArea)
		//{
		//case 0:
		//	memset(keepPointArea, 1, 9 * sizeof(int));
		//	break;
		//case 1:
		//	memset(keepPointArea, 0, 9 * sizeof(int));
		//	keepPointArea[0] = 1;
		//	keepPointArea[3] = 1;
		//	keepPointArea[4] = 1;
		//	keepPointArea[5] = 1;
		//	keepPointArea[6] = 1;
		//	keepPointArea[7] = 1;
		//	break;
		//case 2:
		//	memset(keepPointArea, 0, 9 * sizeof(int));
		//	keepPointArea[0] = 1;
		//	keepPointArea[5] = 1;
		//	keepPointArea[6] = 1;
		//	keepPointArea[7] = 1;
		//	break;
		//case 3:
		//	memset(keepPointArea, 0, 9 * sizeof(int));
		//	keepPointArea[0] = 1;
		//	keepPointArea[5] = 1;
		//	keepPointArea[6] = 1;
		//	keepPointArea[7] = 1;
		//	keepPointArea[8] = 1;
		//	keepPointArea[1] = 1;
		//	break;
		//case 4:
		//	memset(keepPointArea, 0, 9 * sizeof(int));
		//	keepPointArea[0] = 1;
		//	keepPointArea[7] = 1;
		//	keepPointArea[8] = 1;
		//	keepPointArea[1] = 1;
		//	break;
		//case 5:
		//	memset(keepPointArea, 0, 9 * sizeof(int));
		//	keepPointArea[0] = 1;
		//	keepPointArea[7] = 1;
		//	keepPointArea[8] = 1;
		//	keepPointArea[1] = 1;
		//	keepPointArea[2] = 1;
		//	keepPointArea[3] = 1;
		//	break;
		//case 6:
		//	memset(keepPointArea, 0, 9 * sizeof(int));
		//	keepPointArea[0] = 1;
		//	keepPointArea[1] = 1;
		//	keepPointArea[2] = 1;
		//	keepPointArea[3] = 1;
		//	break;
		//case 7:
		//	memset(keepPointArea, 0, 9 * sizeof(int));
		//	keepPointArea[0] = 1;
		//	keepPointArea[1] = 1;
		//	keepPointArea[2] = 1;
		//	keepPointArea[3] = 1;
		//	keepPointArea[4] = 1;
		//	keepPointArea[5] = 1;
		//	break;
		//case 8:
		//	memset(keepPointArea, 0, 9 * sizeof(int));
		//	keepPointArea[0] = 1;
		//	keepPointArea[3] = 1;
		//	keepPointArea[4] = 1;
		//	keepPointArea[5] = 1;
		//	break;
		//default:
		//	break;
		//}
	}

	return spti;
}

inline void VectorSimpleLayer::DrawLineString(int* x, int* y, IGeoSurface* surf, OGRLineString *geometry)
{
	int ptcount = TransferGeometryPoints(x, y, surf, geometry);

	_simpleLineSymbol.Paint(surf, ptcount, x, y, nullptr, nullptr);
}

inline void VectorSimpleLayer::DrawPolygon(int* x, int* y, IGeoSurface* surf, OGRPolygon * geometry)
{
	OGRLinearRing*  exterior = geometry->getExteriorRing();
	if (exterior != nullptr)
	{
		int ptcount = TransferGeometryPoints(x, y, surf, exterior);
		*(x + ptcount) = *x;
		*(y + ptcount) = *y;
		_simpleFillSymbol.Paint(surf, ptcount + 1, x, y, nullptr, nullptr);
		_simpleLineSymbol.Paint(surf, ptcount + 1, x, y, nullptr, nullptr);
	}
}

inline void VectorSimpleLayer::DrawMultiPoint(int* xb, int* yb, IGeoSurface* surf, OGRMultiPoint * geometry)
{
	if (_CT != nullptr)
	{
		geometry->transform(_CT);
	}
	int c = geometry->getNumGeometries();
	for (int i = 0; i < c; i++)
	{
		OGRPoint* pt = (OGRPoint*)geometry->getGeometryRef(i);
		int x = -1;
		int y = -1;
		double spatialX = pt->getX();
		double spatialY = pt->getY();
		OGREnvelope ev;
		ev.MaxX = spatialX;
		ev.MinX = spatialX;
		ev.MaxY = spatialY;
		ev.MinY = spatialY;
		if (_envelope.Contains(ev))
		{
			surf->GetViewPort()->Spatial2Surface(pt->getX(), pt->getY(), &x, &y);
			_simpleMarkerSymbol.Paint(surf, 1, &x, &y, nullptr, nullptr);
		}
	}


}

inline void VectorSimpleLayer::DrawMultiLineString(int* x, int* y, IGeoSurface* surf, OGRMultiLineString * geometry)
{
	int c = geometry->getNumGeometries();
	for (int i = 0; i < c; i++)
	{
		OGRLinearRing*  line = (OGRLinearRing*)geometry->getGeometryRef(i);
		int ptcount = TransferGeometryPoints(x, y, surf, line);

		_simpleLineSymbol.Paint(surf, ptcount, x, y, nullptr, nullptr);
	}
}

inline void VectorSimpleLayer::DrawMultiPolygon(int* x, int* y, IGeoSurface* surf, OGRMultiPolygon *geometry)
{
	int c = geometry->getNumGeometries();
	for (int i = 0; i < c; i++)
	{
		OGRPolygon*  polygon = (OGRPolygon*)geometry->getGeometryRef(i);
		OGRLinearRing*  exterior = polygon->getExteriorRing();
		if (exterior != nullptr)
		{
			int ptcount = TransferGeometryPoints(x, y, surf, exterior);
			*(x + ptcount) = *x;
			*(y + ptcount) = *y;
			_simpleFillSymbol.Paint(surf, ptcount + 1, x, y, nullptr, nullptr);
			_simpleLineSymbol.Paint(surf, ptcount + 1, x, y, nullptr, nullptr);
		}
	}
}



END_NAME_SPACE(tGis, Core)


