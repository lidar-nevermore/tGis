#pragma once

#ifndef __VECTORSIMPLELAYERRENDER_H__
#define __VECTORSIMPLELAYERRENDER_H__


#include "Helper.h"

#include "VectorLayerRender.h"
#include "SimpleMarkerSymbol.h"
#include "SimpleLineSymbol.h"
#include "SimpleFillSymbol.h"

#include <string>

using namespace std;

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

class VectorSimpleLayerProvider;

class TGIS_CORE_API VectorSimpleLayerRender : public VectorLayerRender
{
public:
	VectorSimpleLayerRender(ILayer* layer, int ogrLayer,int geometryField = -1, int labelField = -1);
	~VectorSimpleLayerRender();

private:
	VectorSimpleLayerRender(const VectorSimpleLayerRender &) = delete;
	VectorSimpleLayerRender &operator=(const VectorSimpleLayerRender &) = delete;

private:
	static const char* const _type;

private:
	int _geometryField;
	int _labelField;
	SimpleMarkerSymbol _simpleMarkerSymbol;
	SimpleLineSymbol _simpleLineSymbol;
	SimpleFillSymbol _simpleFillSymbol;

public:
	virtual const char* GetType();
	static const char* S_GetType();

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
