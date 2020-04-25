#include "VectorSimpleLayerRender.h"
#include "IGeoSurface.h"
#include "MyGDALVectorDataset.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include "ogr_spatialref.h"
#include "ogrsf_frmts.h"
#include "ogr_geometry.h"

BEGIN_NAME_SPACE(tGis, Core)

const char* const VectorSimpleLayerRender::_type = "57D45D8B-E28E-4D5F-A588-1A55347A0C68";

const char * VectorSimpleLayerRender::GetType()
{
	return _type;
}

const char * VectorSimpleLayerRender::S_GetType()
{
	return _type;
}

bool VectorSimpleLayerRender::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return VectorLayerRender::IsTypeOf(type);
}

VectorSimpleLayerRender::VectorSimpleLayerRender(ILayer* layer, int ogrLayerIndex, int geometryField, int labelField)
	:VectorLayerRender(layer, ogrLayerIndex)
{
	_geometryField = geometryField;
	_labelField = labelField;
}


VectorSimpleLayerRender::~VectorSimpleLayerRender()
{
}


void VectorSimpleLayerRender::Paint(IGeoSurface * surf)
{
	OGRFeature* feature = nullptr;
	OGRGeometry* geometry = nullptr;
	LayerRenderBuffer rbuf;
	unsigned char* surfBuffer = rbuf._acquiredSurfaceBuffer;
	unsigned char* pixBuffer = rbuf._acquiredDatasetBuffer;

	int* x = (int*)surfBuffer;
	int* y = (int*)pixBuffer;
	PrepareCT();
	_ogrLayer->ResetReading();
	do
	{
		feature = _ogrLayer->GetNextFeature();
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
}


inline OGRGeometry * VectorSimpleLayerRender::GetGeometry(OGRFeature *feature)
{
	if (_geometryField < 0)
		return feature->GetGeometryRef();
	else
		return feature->GetGeomFieldRef(_geometryField);
}

inline const char * VectorSimpleLayerRender::GetLabel(OGRFeature *)
{
	return nullptr;
}

inline void VectorSimpleLayerRender::DrawPoint(int* xb, int* yb, IGeoSurface* surf, OGRPoint *geometry)
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
		_simpleMarkerSymbol.Paint(surf, 1, &x, &y);
	}
}

inline int VectorSimpleLayerRender::TransferGeometryPoints(int * x, int * y, IGeoSurface* surf, OGRSimpleCurve *geometry)
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
	int omitedPointCount = 0; //����һ�α����㵽�����ų��ĵ���Ŀ
	int pointArea; //��ǰ���������ڵ�������ʾ����+�����������·ֱ���012345678
	int keepPointArea[9];//��һ�����豣������ֵ���������λ�ñ�ʾ��������ֵΪ1��ʾ����

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

inline void VectorSimpleLayerRender::DrawLineString(int* x, int* y, IGeoSurface* surf, OGRLineString *geometry)
{
	int ptcount = TransferGeometryPoints(x, y, surf, geometry);

	_simpleLineSymbol.Paint(surf, ptcount, x, y);
}

inline void VectorSimpleLayerRender::DrawPolygon(int* x, int* y, IGeoSurface* surf, OGRPolygon * geometry)
{
	OGRLinearRing*  exterior = geometry->getExteriorRing();
	if (exterior != nullptr)
	{
		int ptcount = TransferGeometryPoints(x, y, surf, exterior);
		*(x + ptcount) = *x;
		*(y + ptcount) = *y;
		//_simpleFillSymbol.Paint(surf, ptcount + 1, x, y);
		_simpleLineSymbol.SetColor(0, 255, 255, 255);
		_simpleLineSymbol.Paint(surf, ptcount + 1, x, y);
	}
}

inline void VectorSimpleLayerRender::DrawMultiPoint(int* xb, int* yb, IGeoSurface* surf, OGRMultiPoint * geometry)
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
			_simpleMarkerSymbol.Paint(surf, 1, &x, &y);
		}
	}


}

inline void VectorSimpleLayerRender::DrawMultiLineString(int* x, int* y, IGeoSurface* surf, OGRMultiLineString * geometry)
{
	int c = geometry->getNumGeometries();
	for (int i = 0; i < c; i++)
	{
		OGRLinearRing*  line = (OGRLinearRing*)geometry->getGeometryRef(i);
		int ptcount = TransferGeometryPoints(x, y, surf, line);

		_simpleLineSymbol.Paint(surf, ptcount, x, y);
	}
}

inline void VectorSimpleLayerRender::DrawMultiPolygon(int* x, int* y, IGeoSurface* surf, OGRMultiPolygon *geometry)
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
			//_simpleFillSymbol.Paint(surf, ptcount + 1, x, y);
			_simpleLineSymbol.Paint(surf, ptcount + 1, x, y);
		}
	}
}



END_NAME_SPACE(tGis, Core)

