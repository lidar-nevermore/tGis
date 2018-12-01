#pragma once

#ifndef __GEOSURFACE_H__
#define __GEOSURFACE_H__

#include "Helper.h"

#include "IGeoSurface.h"

#include "gdal.h"

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)
class TGIS_API GeoSurface : IGeoSurface
{
protected:
	GeoSurface();
	virtual ~GeoSurface();

public:
	virtual void SetViewScale(double scale);
	virtual void GetViewScale(double* scale);
	virtual void SetViewCenter(double spatialCenterX, double spatialCenterY);
	virtual void GetViewCenter(double* spatialCenterX, double* spatialCenterY);
	virtual void GetViewSize(int* surfW, int* surfH);
	virtual void SetViewPort(double spatialCenterX, double spatialCenterY, double scale);
	virtual void GetViewPort(double *spatialCenterX, double *spatialCenterY, double* scale);
	virtual void IncludeEnvelope(const OGREnvelope*);
	virtual void IncludeEnvelope(double spatialLeft, double spatialTop, double spatialRight, double spatialBottom);
	virtual void Surface2Spatial(int surfX, int surfY, double *spatialX, double *spatialY);
	virtual void Spatial2Surface(double spatialX, double spatialY, double *surfX, double *surfY);
	virtual void Spatial2Surface(double spatialX, double spatialY, int *surfX, int *surfY);
	virtual const OGREnvelope* GetEnvelope();
	virtual const OGRSpatialReference* GetSpatialReference();
	virtual void SetSpatialReference(const OGRSpatialReference*);

protected:
	void SwithSurface();
	void UpdateViewPort();

protected:
	OGREnvelope _envelope;
	OGRSpatialReference* _spatialRef;
	//screen spatial pos
	double _spatialCenterX;
	double _spatialCenterY;
	double _scale;
	double _spatialLeft;
	double _spatialTop;
	//screen size in pix
	int _surfWidth;
	int _surfHeight;

	//visible spatial area of off screen surface for presenting
	double _osSurf4PresentScale;
	double _osSurf4PresentSpatialLeft;
	double _osSurf4PresentSpatialTop;
	double _osSurf4PresentSpatialRight;
	double _osSurf4PresentSpatialBottom;
	//visible pix area of off screen surface for presenting
	int _osSurf4PresentWidth;
	int _osSurf4PresentHeight;
	//present rect in screen of off screen surface for presenting
	int _osSurfPresentPosX;
	int _osSurfPresentPosY;
	int _osSurfPresentWidth;
	int _osSurfPresentHeight;
};

END_NAME_SPACE(tGis, Core)


#endif
