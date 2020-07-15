#include "VectorUniformLayerRender.h"
#include "IGeoSurface.h"
#include "MyGDALVectorDataset.h"

#include "IMarkerSymbol.h"
#include "ILineSymbol.h"
#include "IFillSymbol.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include "ogr_spatialref.h"
#include "ogrsf_frmts.h"
#include "ogr_geometry.h"

BEGIN_NAME_SPACE(tGis, Core)

const char* const VectorUniformLayerRender::_type = "57D45D8B-E28E-4D5F-A588-1A55347A0C68";

const char * VectorUniformLayerRender::GetType()
{
	return _type;
}

const char * VectorUniformLayerRender::S_GetType()
{
	return _type;
}

bool VectorUniformLayerRender::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return VectorLayerRender::IsTypeOf(type);
}

VectorUniformLayerRender::VectorUniformLayerRender(ILayer* layer, int ogrLayerIndex, int geometryField)
	:VectorLayerRender(layer, ogrLayerIndex)
{
	_geometryField = geometryField;
	_markerSymbol = nullptr;
	_lineSymbol = nullptr;
	_fillSymbol = nullptr;
}


VectorUniformLayerRender::~VectorUniformLayerRender()
{
	if (_markerSymbol != nullptr)
		_markerSymbol->Release();
	if (_lineSymbol != nullptr)
		_lineSymbol->Release();
	//逻辑运算符真应该把右值放到左边
	if (nullptr != _fillSymbol)
		_fillSymbol->Release();
}



IMarkerSymbol * VectorUniformLayerRender::GetMarkerSymbol()
{
	return _markerSymbol;
}

void VectorUniformLayerRender::SetMarkerSymbol(IMarkerSymbol * sym)
{
	if (_markerSymbol != sym)
	{
		if(_markerSymbol != nullptr)
		    _markerSymbol->Release();
		_markerSymbol = sym;
		if (_markerSymbol != nullptr)
			_markerSymbol->Reference();
	}
}

ILineSymbol * VectorUniformLayerRender::GetLineSymbol()
{
	return _lineSymbol;
}

void VectorUniformLayerRender::SetLineSymbol(ILineSymbol * sym)
{
	if (_lineSymbol != sym)
	{
		if (_lineSymbol != nullptr)
			_lineSymbol->Release();
		_lineSymbol = sym;
		if (_lineSymbol != nullptr)
			_lineSymbol->Reference();
	}
}

IFillSymbol * VectorUniformLayerRender::GetFillSymbol()
{
	return _fillSymbol;
}

void VectorUniformLayerRender::SetFillSymbol(IFillSymbol * sym)
{
	if (_fillSymbol != sym)
	{
		if (_fillSymbol != nullptr)
			_fillSymbol->Release();
		_fillSymbol = sym;
		if (_fillSymbol != nullptr)
			_fillSymbol->Reference();
	}
}

void VectorUniformLayerRender::Paint(IGeoSurface * surf)
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


inline OGRGeometry * VectorUniformLayerRender::GetGeometry(OGRFeature *feature)
{
	if (_geometryField < 0)
		return feature->GetGeometryRef();
	else
		return feature->GetGeomFieldRef(_geometryField);
}

inline const char * VectorUniformLayerRender::GetLabel(OGRFeature *)
{
	return nullptr;
}

inline void VectorUniformLayerRender::DrawPoint(int* xb, int* yb, IGeoSurface* surf, OGRPoint *geometry)
{
	if (_markerSymbol == nullptr)
		return;

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
		_markerSymbol->Paint(surf, 1, &x, &y);
	}
}

inline int VectorUniformLayerRender::TransferGeometryPoints(int * x, int * y, IGeoSurface* surf, OGRSimpleCurve *geometry)
{
	if (_CT != nullptr)
	{
		geometry->transform(_CT);
	}

	int surfW;
	int surfH;
	surf->GetSize(&surfW, &surfH);
	int ptcount = geometry->getNumPoints();
	int pti = 0;
	int spti = 0;

	int presx = INT_MAX;
	int presy = INT_MAX;
	int omitedPointCount = 0; //从上一次保留点到现在排除的点数目
	int pointArea; //当前保留点所在的区域；显示区域+以外左上右下分别是012345678
	int keepPointArea[9];//下一次所需保留点出现的区域，数组位置表示区域，其中值为1表示保留

	memset(keepPointArea, 1, 9 * sizeof(int));

	while (pti < ptcount)
	{
		int sx;
		int sy;
		surf->GetViewPort()->Spatial2Surface(geometry->getX(pti), geometry->getY(pti), &sx, &sy);
		pti++;

		//坐标值相等的点只保留一个
		if (presx == sx && presx == sy)
			continue;

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
			if (omitedPointCount > 0)
			{
				*(x + spti) = presx;
				*(y + spti) = presy;
				spti++;
				*(x + spti) = sx;
				*(y + spti) = sy;
			}
			spti++;
			omitedPointCount = 0;
		}
		else
		{
			omitedPointCount++;
		}


		presx = sx;
		presy = sy;
		memset(keepPointArea, 1, 9 * sizeof(int));
		if(curPointArea != 0)
			keepPointArea[curPointArea] = 0;
		pointArea = curPointArea;
	}

	return spti;
}

inline void VectorUniformLayerRender::DrawLineString(int* x, int* y, IGeoSurface* surf, OGRLineString *geometry)
{
	if (_lineSymbol == nullptr)
		return;

	int ptcount = TransferGeometryPoints(x, y, surf, geometry);

	_lineSymbol->Paint(surf, ptcount, x, y);
}

inline void VectorUniformLayerRender::DrawPolygon(int* x, int* y, IGeoSurface* surf, OGRPolygon * geometry)
{
	if (_lineSymbol == nullptr)
		return;

	OGRLinearRing*  exterior = geometry->getExteriorRing();
	if (exterior != nullptr)
	{
		int ptcount = TransferGeometryPoints(x, y, surf, exterior);
		*(x + ptcount) = *x;
		*(y + ptcount) = *y;
		if (_fillSymbol != nullptr)
		{
			_fillSymbol->Paint(surf, ptcount + 1, x, y);
			//_fillSymbol->BeginPaint(surf);
			//_fillSymbol->BeginContour(surf);
			//_fillSymbol->AppendVertex(ptcount + 1, x, y);
			//_fillSymbol->EndContour(surf);
			//_fillSymbol->EndPaint(surf);
		}
		_lineSymbol->Paint(surf, ptcount + 1, x, y);
	}
}

inline void VectorUniformLayerRender::DrawMultiPoint(int* xb, int* yb, IGeoSurface* surf, OGRMultiPoint * geometry)
{
	if (_markerSymbol == nullptr)
		return;

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
			_markerSymbol->Paint(surf, 1, &x, &y);
		}
	}


}

inline void VectorUniformLayerRender::DrawMultiLineString(int* x, int* y, IGeoSurface* surf, OGRMultiLineString * geometry)
{
	if (_lineSymbol == nullptr)
		return;

	int c = geometry->getNumGeometries();
	for (int i = 0; i < c; i++)
	{
		OGRLinearRing*  line = (OGRLinearRing*)geometry->getGeometryRef(i);
		int ptcount = TransferGeometryPoints(x, y, surf, line);

		_lineSymbol->Paint(surf, ptcount, x, y);
	}
}

inline void VectorUniformLayerRender::DrawMultiPolygon(int* x, int* y, IGeoSurface* surf, OGRMultiPolygon *geometry)
{
	if (_lineSymbol == nullptr)
		return;

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
			if (_fillSymbol != nullptr)
				_fillSymbol->Paint(surf, ptcount + 1, x, y);
			_lineSymbol->Paint(surf, ptcount + 1, x, y);
		}
	}
}

END_NAME_SPACE(tGis, Core)

