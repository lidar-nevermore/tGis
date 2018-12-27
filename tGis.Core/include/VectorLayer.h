#pragma once

#ifndef __VECTORLAYER_H__
#define __VECTORLAYER_H__


#include "Helper.h"

#include "Layer.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"

#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

class MyGDALVectorDataset;

class TGIS_CORE_API VectorLayer : public Layer
{
protected:
	VectorLayer(ILayerProvider* provider);
	VectorLayer(ILayerProvider* provider, MyGDALVectorDataset* vector, OGRLayer* layer);

public:
	~VectorLayer();

protected:
	void SetOGRLayer(MyGDALVectorDataset* vector, OGRLayer* layer);

public:
	virtual const OGREnvelope* GetEnvelope();
	virtual const OGRSpatialReference* GetSpatialReference();
	virtual bool CanTransformTo(const OGRSpatialReference*);
	virtual IDataset* GetDataset();

protected:
	inline void PrepareCT();

protected:
	MyGDALVectorDataset* _vector;
	OGRLayer* _layer;
	OGREnvelope _envelope;
	OGRSpatialReference* _spatialRef;
	OGRSpatialReference* _mapSpatialRef;
	OGRCoordinateTransformation* _CT;
};


END_NAME_SPACE(tGis, Core)

#endif
