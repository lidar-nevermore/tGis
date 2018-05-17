#pragma once

#ifndef __DATASOURCEPROVIDERPOOL_H__
#define __DATASOURCEPROVIDERPOOL_H__

#include "Helper.h"

class OGREnvelope;
class OGRSpatialReference;

BEGIN_NAME_SPACE(tGis, Core)

struct TGISCORE_API IGeoSurface
{
	virtual void SetViewResolution(double resolution) = 0;
	virtual void SetViewCenter(double spatialCenterX, double spatialCenterY) = 0;
	virtual void SetViewSize(int surfW, int surfH) = 0;
	virtual void SetViewPort(double spatialCenterX, double spatialCenterY, int surfW, int surfH, double resolution) = 0;
	virtual void GetViewPort(double *spatialCenterX, double *spatialCenterY, int *surfW, int *surfH, double *resolution) = 0;

	virtual void Surface2Spatial(int surfX, int surfY, double *spatialX, double *spatialY) = 0;
	virtual void Spatial2Surface(double spatialX, double spatialY, double *surfX, double *surfY) = 0;
	virtual const OGREnvelope* GetEnvelope() = 0;
	virtual const OGRSpatialReference* GetSpatialReference() = 0;
	virtual void SetSpatialReference(const OGRSpatialReference*) = 0;

	virtual void PresentSurface() = 0;
	virtual void SwithSurface() = 0;

	virtual void FillRect(int surfX, int surfY, int width, int height, unsigned char R, unsigned char G, unsigned char B, unsigned char A) = 0;
	virtual void DrawImage(const unsigned char* buf, int surfX, int surfY, int width, int height) = 0;

	virtual ~IGeoSurface() {};
};


END_NAME_SPACE(tGis, Core)


#endif
