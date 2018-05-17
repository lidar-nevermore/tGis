#include "GeoSurface.h"
#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"

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

void GeoSurface::SetViewSize(int surfW, int surfH)
{
	_surfWidth = surfW;
	_surfHeight = surfH;
	UpdateViewPort();
}

void GeoSurface::SetViewPort(double spatialCenterX, double spatialCenterY, int surfW, int surfH, double resolution)
{
	_spatialCenterX = spatialCenterX;
	_spatialCenterY = spatialCenterY;
	_surfWidth = surfW;
	_surfHeight = surfH;
	_resolution = resolution;
	UpdateViewPort();
}

void GeoSurface::GetViewPort(double *spatialCenterX, double *spatialCenterY, int *surfW, int *surfH, double *resolution)
{
	if (spatialCenterX != nullptr)
		*spatialCenterX = _spatialCenterX;

	if (spatialCenterY != nullptr)
		*spatialCenterY = _spatialCenterY;

	if (surfW != nullptr)
		*surfW = _surfWidth;

	if (surfH != nullptr)
		*surfH = _surfHeight;

	if (resolution != nullptr)
		*resolution = _resolution;
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

	_osSurfPresentPosX = (int)my_round(osSurfPresentPosLeft, 0);
	_osSurfPresentPosY = (int)my_round(osSurfPresentPosTop, 0);
	_osSurfPresentWidth = (int)my_round(osSurfPresentPosRight - osSurfPresentPosLeft, 0);
	_osSurfPresentHeight = (int)my_round(osSurfPresentPosBottom - osSurfPresentPosTop, 0);
}

END_NAME_SPACE(tGis, Core)