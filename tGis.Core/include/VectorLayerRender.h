#pragma once

#ifndef __VECTORLAYERRENDER_H__
#define __VECTORLAYERRENDER_H__


#include "Helper.h"

#include "LayerRender.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"

BEGIN_NAME_SPACE(tGis, Core)

class MyGDALVectorDataset;

class TGIS_CORE_API VectorLayerRender : public LayerRender
{
public:
	VectorLayerRender(ILayer* layer, int ogrLayer);
	~VectorLayerRender();

public:
	virtual const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);

private:
	static const char* const _type;

public:
	virtual const OGREnvelope* GetEnvelope();
	virtual const OGRSpatialReference* GetSpatialReference();
	virtual bool CanTransformTo(const OGRSpatialReference*);

	virtual int GetOGRLayer()
	{
		return _ogrLayerIndex;
	}

protected:
	inline void PrepareCT();

protected:
	MyGDALVectorDataset* _vector;
	OGRLayer* _ogrLayer;
	int _ogrLayerIndex;
	OGREnvelope _envelope;
	OGRSpatialReference* _spatialRef;
	OGRSpatialReference* _mapSpatialRef;
	OGRCoordinateTransformation* _CT;
};


END_NAME_SPACE(tGis, Core)

#endif
