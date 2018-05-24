#pragma once

#ifndef __I_GEOSURFACE_H__
#define __I_GEOSURFACE_H__

#include "Helper.h"

#include "ISurface.h"

class OGREnvelope;
class OGRSpatialReference;

BEGIN_NAME_SPACE(tGis, Core)

struct TGISCORE_API IGeoSurface : public ISurface
{
	virtual void SetViewResolution(double resolution) = 0;
	virtual void SetViewCenter(double spatialCenterX, double spatialCenterY) = 0;
	virtual void SetViewPort(double spatialCenterX, double spatialCenterY, int surfW, int surfH, double resolution) = 0;
	virtual void GetViewPort(double *spatialCenterX, double *spatialCenterY, int *surfW, int *surfH, double *resolution) = 0;

	virtual void Surface2Spatial(int surfX, int surfY, double *spatialX, double *spatialY) = 0;
	virtual void Spatial2Surface(double spatialX, double spatialY, double *surfX, double *surfY) = 0;
	virtual void Spatial2Surface(double spatialX, double spatialY, int *surfX, int *surfY) = 0;
	virtual const OGREnvelope* GetEnvelope() = 0;
	virtual const OGRSpatialReference* GetSpatialReference() = 0;
	virtual void SetSpatialReference(const OGRSpatialReference*) = 0;

	virtual void PresentSurface() = 0;
	virtual void SwithSurface() = 0;

	IGeoSurface() {};
	virtual ~IGeoSurface() {};
private:
	IGeoSurface(const IGeoSurface &) = delete;
	IGeoSurface &operator=(const IGeoSurface &) = delete;
};

typedef IGeoSurface* IGeoSurfacePtr;

END_NAME_SPACE(tGis, Core)


#endif
