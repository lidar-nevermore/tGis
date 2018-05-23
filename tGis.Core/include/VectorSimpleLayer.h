#pragma once

#ifndef __VECTORSIMPLELAYER_H__
#define __VECTORSIMPLELAYER_H__


#include "Helper.h"

#include "VectorLayer.h"
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

BEGIN_NAME_SPACE(tGis, Core)

class TGISCORE_API VectorSimpleLayer : public VectorLayer
{
public:
	VectorSimpleLayer();
	VectorSimpleLayer(OGRLayer* layer,int geometryField = -1, int labelField = -1);
	~VectorSimpleLayer();

private:
	using VectorLayer::SetOGRLayer;

public:
	void SetOGRLayer(OGRLayer* layer, int geometryField, int labelField = -1);

private:
	static const char* const _type;

private:
	OGRSpatialReference* _surfSpatialRef;
	OGRCoordinateTransformation* _CT;
	int _geometryField;
	int _labelField;
	SimpleMarkerSymbol _simpleMarkerSymbol;
	SimpleLineSymbol _simpleLineSymbol;
	SimpleFillSymbol _simpleFillSymbol;

public:
	virtual const char* GetType();

	virtual void Paint(IGeoSurface*);

private:
	inline void PrepareCT(IGeoSurface*);

	inline OGRGeometry* GetGeometry(OGRFeature*);
	inline const char* GetLabel(OGRFeature*);

	inline void DrawPoint(IGeoSurface*, OGRPoint*);
	inline void DrawLineString(IGeoSurface*, OGRLineString*);
	inline void DrawPolygon(IGeoSurface*, OGRPolygon*);
	inline void DrawMultiPoint(IGeoSurface*, OGRMultiPoint*);
	inline void DrawMultiLineString(IGeoSurface*, OGRMultiLineString*);
	inline void DrawMultiPolygon(IGeoSurface*, OGRMultiPolygon*);
};


END_NAME_SPACE(tGis, Core)

#endif