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

class TGISCORE_API VectorLayer : public Layer
{
public:
	VectorLayer();
	VectorLayer(OGRLayer*);
	~VectorLayer();

protected:
	void SetOGRLayer(OGRLayer* layer);

public:
	virtual const OGREnvelope* GetEnvelope();
	virtual const OGRSpatialReference* GetSpatialReference();

protected:
	OGRLayer* _layer;
	OGREnvelope _envelope;
	OGRSpatialReference* _spatialRef;
};


END_NAME_SPACE(tGis, Core)

#endif
