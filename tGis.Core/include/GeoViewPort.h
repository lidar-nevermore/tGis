#pragma once

#ifndef __GEOVIEWPORT_H__
#define __GEOVIEWPORT_H__

#include "Helper.h"
#include "Event.h"

class OGREnvelope;
class OGRSpatialReference;

BEGIN_NAME_SPACE(tGis, Core)

class GeoViewPort;

template struct TGIS_CORE_API IEventHandler<GeoViewPort*>;
template class TGIS_CORE_API Event<GeoViewPort*>;

class TGIS_CORE_API GeoViewPort
{
	friend class MapWidget;
	friend class GeoSurface;
public:
	GeoViewPort();
	~GeoViewPort();
	GeoViewPort(const GeoViewPort &);
	GeoViewPort &operator=(const GeoViewPort &);

public:
	Event<GeoViewPort*> ChangedEvent;

public:
	void GetSurfaceSize(int* surfW, int* surfH) const;

private:
	void SetSurfaceSize(int surfW, int surfH);

public:
	const OGRSpatialReference* GetSpatialReference() const;

private:
	void SetSpatialReference(const OGRSpatialReference*);

public:
	void GetSpatialCenter(double* spatialCenterX, double* spatialCenterY) const;
	void SetSpatialCenter(double spatialCenterX, double spatialCenterY);

	void IncludeEnvelope(const OGREnvelope*);
	void IncludeEnvelope(double spatialLeft, double spatialTop, double spatialRight, double spatialBottom);
	void GetEnvelope(OGREnvelope* extent) const;
	void GetEnvelope(double* spatialLeft, double* spatialTop, double* spatialRight, double* spatialBottom) const;

	void GetViewScale(double* scale) const;
	void SetViewScale(double scale);

private:
	void UpdateViewPort();

public:
	virtual void Surface2Spatial(int surfX, int surfY, double *spatialX, double *spatialY) const;
	virtual void Spatial2Surface(double spatialX, double spatialY, double *surfX, double *surfY) const;
	virtual void Spatial2Surface(double spatialX, double spatialY, int *surfX, int *surfY) const;

private:
	//spatial extent of the viewport
	//指向map对象的_spatialRef，不必释放
	OGRSpatialReference* _spatialRef;
	double _spatialCenterX;
	double _spatialCenterY;
	double _spatialLeft;
	double _spatialTop;
	double _spatialRight;
	double _spatialBottom;

	//device size of the viewport in pix
	int _surfWidth;
	int _surfHeight;

	//spatial length per device unit
	double _scale;
};

END_NAME_SPACE(tGis, Core)


#endif
