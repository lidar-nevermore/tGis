#include "GeoSurface.h"
#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include <algorithm>
#include <cmath>

BEGIN_NAME_SPACE(tGis, Core)

GeoSurface::GeoSurface()
{
	_spatialRef = nullptr;
}


GeoSurface::~GeoSurface()
{
	if (_spatialRef != nullptr)
	{
		_spatialRef->Release();
	}
}

void GeoSurface::SetViewResolution(double resolution)
{
	_resolution = resolution;
	UpdateViewPort();
}

void GeoSurface::SetViewCenter(double spatialCenterX, double spatialCenterY)
{
	_spatialCenterX = spatialCenterX;
	_spatialCenterY = spatialCenterY;
	UpdateViewPort();
}

void GeoSurface::GetViewSize(int * surfW, int * surfH)
{
	*surfW = _surfWidth;
	*surfH = _surfHeight;
}

void GeoSurface::SetViewPort(double spatialCenterX, double spatialCenterY, double resolution)
{
	_spatialCenterX = spatialCenterX;
	_spatialCenterY = spatialCenterY;
	_resolution = resolution;
	UpdateViewPort();
}

void GeoSurface::GetViewPort(double *spatialCenterX, double *spatialCenterY, double *resolution)
{
	if (spatialCenterX != nullptr)
		*spatialCenterX = _spatialCenterX;

	if (spatialCenterY != nullptr)
		*spatialCenterY = _spatialCenterY;

	if (resolution != nullptr)
		*resolution = _resolution;
}

void GeoSurface::IncludeEnvelope(const OGREnvelope * envelope)
{
	double height = envelope->MaxY - envelope->MinY;
	double width = envelope->MaxX - envelope->MinX;
	double resolution = std::max(abs(height / _surfHeight), abs(width / _surfWidth));

	_resolution = resolution;
	SetViewCenter((envelope->MaxX + envelope->MinX) / 2.0, (envelope->MaxY + envelope->MinY) / 2.0);
}

void GeoSurface::IncludeEnvelope(double spatialLeft, double spatialTop, double spatialRight, double spatialBottom)
{
	double height = spatialRight - spatialLeft;
	double width = spatialBottom - spatialTop;
	double resolution = std::max(abs(height / _surfHeight), abs(width / _surfWidth));

	_resolution = resolution;
	SetViewCenter((spatialRight + spatialLeft) / 2.0, (spatialBottom + spatialTop) / 2.0);
}

void GeoSurface::Surface2Spatial(int surfX, int surfY, double * spatialX, double * spatialY)
{
	*spatialX = _spatialLeft + surfX*_resolution;
	*spatialY = _spatialTop - surfY*_resolution;
}

void GeoSurface::Spatial2Surface(double spatialX, double spatialY, double * surfX, double * surfY)
{
	*surfX = (spatialX - _spatialLeft) / _resolution;
	*surfY = (_spatialTop - spatialY) / _resolution;
}

void GeoSurface::Spatial2Surface(double spatialX, double spatialY, int * surfX, int * surfY)
{
	*surfX = (int)_tgis_round((spatialX - _spatialLeft) / _resolution, 0);
	*surfY = (int)_tgis_round((_spatialTop - spatialY) / _resolution, 0);
}

const OGREnvelope * GeoSurface::GetEnvelope()
{
	return &_envelope;
}

const OGRSpatialReference * GeoSurface::GetSpatialReference()
{
	return _spatialRef;
}

void GeoSurface::SetSpatialReference(const OGRSpatialReference * ref)
{
	_spatialRef = const_cast<OGRSpatialReference*>(ref);
	if (_spatialRef != nullptr)
	{
		_spatialRef->Reference();
	}
}

void GeoSurface::SwithSurface()
{
	_osSurf4PresentWidth = _surfWidth;
	_osSurf4PresentHeight = _surfHeight;
	_osSurf4PresentResolution = _resolution;
	_osSurf4PresentSpatialLeft = _spatialLeft;
	_osSurf4PresentSpatialTop = _spatialTop;
	_osSurf4PresentSpatialRight = _envelope.MaxX;
	_osSurf4PresentSpatialBottom = _envelope.MinY;
	_osSurfPresentPosX = 0;
	_osSurfPresentPosY = 0;
	_osSurfPresentWidth = _surfWidth;
	_osSurfPresentHeight = _surfHeight;
}

void GeoSurface::UpdateViewPort()
{
	double halfW = _resolution*_surfWidth / 2.0;
	double halfH = _resolution*_surfHeight / 2.0;
	_spatialLeft = _spatialCenterX - halfW;
	_spatialTop = _spatialCenterY + halfH;
	_envelope.MinX = _spatialLeft;
	_envelope.MaxY = _spatialTop;
	_envelope.MaxX = _spatialCenterX + halfW;
	_envelope.MinY = _spatialCenterY - halfH;

	double osSurfPresentPosLeft;
	double osSurfPresentPosTop;
	double osSurfPresentPosRight;
	double osSurfPresentPosBottom;
	Spatial2Surface(_osSurf4PresentSpatialLeft, _osSurf4PresentSpatialTop, &osSurfPresentPosLeft, &osSurfPresentPosTop);
	Spatial2Surface(_osSurf4PresentSpatialRight, _osSurf4PresentSpatialBottom, &osSurfPresentPosRight, &osSurfPresentPosBottom);

	_osSurfPresentPosX = (int)_tgis_round(osSurfPresentPosLeft, 0);
	_osSurfPresentPosY = (int)_tgis_round(osSurfPresentPosTop, 0);
	_osSurfPresentWidth = (int)_tgis_round(osSurfPresentPosRight - osSurfPresentPosLeft, 0);
	_osSurfPresentHeight = (int)_tgis_round(osSurfPresentPosBottom - osSurfPresentPosTop, 0);
}

END_NAME_SPACE(tGis, Core)