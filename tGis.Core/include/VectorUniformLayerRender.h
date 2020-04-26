#pragma once

#ifndef __VectorUniformLayerRender_H__
#define __VectorUniformLayerRender_H__


#include "Helper.h"

#include "VectorLayerRender.h"
#include "SimpleMarkerSymbol.h"
#include "SimpleLineSymbol.h"
#include "SimpleFillSymbol.h"

class OGRFeature;
class OGRPoint;
class OGRLineString;
class OGRPolygon;
class OGRMultiPoint;
class OGRMultiLineString;
class OGRMultiPolygon;
class OGRCoordinateTransformation;
class OGRSimpleCurve;

BEGIN_NAME_SPACE(tGis, Core)


class TGIS_CORE_API VectorUniformLayerRender : public VectorLayerRender
{
public:
	virtual const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);

private:
	static const char* const _type;

public:
	VectorUniformLayerRender(ILayer* layer, int ogrLayer,int geometryField = -1, int labelField = -1);
	~VectorUniformLayerRender();

private:
	VectorUniformLayerRender(const VectorUniformLayerRender &) = delete;
	VectorUniformLayerRender &operator=(const VectorUniformLayerRender &) = delete;

private:
	int _geometryField;
	int _labelField;
	SimpleMarkerSymbol _simpleMarkerSymbol;
	SimpleLineSymbol _simpleLineSymbol;
	SimpleFillSymbol _simpleFillSymbol;

public:
	virtual void Paint(IGeoSurface*);

private:
	inline OGRGeometry* GetGeometry(OGRFeature*);
	inline const char* GetLabel(OGRFeature*);

	inline void DrawPoint(int* x, int* y, IGeoSurface*, OGRPoint*);
	inline int TransferGeometryPoints(int* x, int* y, IGeoSurface*, OGRSimpleCurve*);
	inline void DrawLineString(int* x, int* y, IGeoSurface*, OGRLineString*);
	inline void DrawPolygon(int* x, int* y, IGeoSurface*, OGRPolygon*);
	inline void DrawMultiPoint(int* x, int* y, IGeoSurface*, OGRMultiPoint*);
	inline void DrawMultiLineString(int* x, int* y, IGeoSurface*, OGRMultiLineString*);
	inline void DrawMultiPolygon(int* x, int* y, IGeoSurface*, OGRMultiPolygon*);

};


END_NAME_SPACE(tGis, Core)

#endif
