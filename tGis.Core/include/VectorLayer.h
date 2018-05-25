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

class TGISCORE_API VectorLayer : public Layer
{
public:
	VectorLayer();
	VectorLayer(MyGDALVectorDataset* vector, OGRLayer* layer);
	~VectorLayer();

protected:
	inline void SetOGRLayer(MyGDALVectorDataset* vector, OGRLayer* layer);

public:
	virtual const OGREnvelope* GetEnvelope();
	virtual const OGRSpatialReference* GetSpatialReference();
	virtual IDataset* GetDataset();

protected:
	MyGDALVectorDataset* _vector;
	OGRLayer* _layer;
	OGREnvelope _envelope;
	OGRSpatialReference* _spatialRef;
};


END_NAME_SPACE(tGis, Core)

#endif
