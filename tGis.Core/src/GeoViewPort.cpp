#include "GeoViewPort.h"

#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"

BEGIN_NAME_SPACE(tGis, Core)


GeoViewPort::GeoViewPort()
{
	_spatialRef = nullptr;
	_spatialCenterX = 0;
	_spatialCenterY = 0;
	_spatialLeft = 0;
	_spatialTop = 0;
	_spatialRight = 0;
	_spatialBottom = 0;
	_surfWidth = 0;
	_surfHeight = 0;
	_scale = 1;
}


GeoViewPort::~GeoViewPort()
{
	if (_spatialRef != nullptr)
	{
		_spatialRef->Dereference();
	}
}

GeoViewPort::GeoViewPort(const GeoViewPort & viewPort)
{
	*this = viewPort;
	//_spatialRef = viewPort._spatialRef;
	//if (_spatialRef != nullptr)
	//{
	//	_spatialRef->Reference();
	//}
	//_spatialCenterX = viewPort._spatialCenterX;
	//_spatialCenterY = viewPort._spatialCenterY;
	//_spatialLeft = viewPort._spatialLeft;
	//_spatialTop = viewPort._spatialTop;
	//_spatialRight = viewPort._spatialRight;
	//_spatialBottom = viewPort._spatialBottom;
	//_surfWidth = viewPort._surfWidth;
	//_surfHeight = viewPort._surfHeight;
	//_scale = viewPort._scale;
}

GeoViewPort & GeoViewPort::operator=(const GeoViewPort & viewPort)
{
	if (_spatialRef != nullptr)
	{
		_spatialRef->Dereference();
	}
	_spatialRef = viewPort._spatialRef;
	if (_spatialRef != nullptr)
	{
		_spatialRef->Reference();
	}
	_spatialCenterX = viewPort._spatialCenterX;
	_spatialCenterY = viewPort._spatialCenterY;
	_spatialLeft = viewPort._spatialLeft;
	_spatialTop = viewPort._spatialTop;
	_spatialRight = viewPort._spatialRight;
	_spatialBottom = viewPort._spatialBottom;
	_surfWidth = viewPort._surfWidth;
	_surfHeight = viewPort._surfHeight;
	_scale = viewPort._scale;

	return *this;
}

void GeoViewPort::GetSurfaceSize(int * surfW, int * surfH) const
{
	*surfW = _surfWidth;
	*surfH = _surfHeight;
}

void GeoViewPort::SetSurfaceSize(int surfW, int surfH)
{
	_surfWidth = surfW;
	_surfHeight = surfH;
	UpdateViewPort();
}

const OGRSpatialReference * GeoViewPort::GetSpatialReference() const
{
	return _spatialRef;
}

void GeoViewPort::SetSpatialReference(const OGRSpatialReference * spatialRef)
{
	if (_spatialRef != nullptr)
	{
		_spatialRef->Dereference();
	}
	_spatialRef = const_cast<OGRSpatialReference*>(spatialRef);
	if (_spatialRef != nullptr)
	{
		_spatialRef->Reference();
	}
}

void GeoViewPort::GetSpatialCenter(double * spatialCenterX, double * spatialCenterY) const
{
	*spatialCenterX = _spatialCenterX;
	*spatialCenterY = _spatialCenterY;
}

void GeoViewPort::SetSpatialCenter(double spatialCenterX, double spatialCenterY)
{
	_spatialCenterX = spatialCenterX;
	_spatialCenterY = spatialCenterY;
	UpdateViewPort();
}

void GeoViewPort::IncludeEnvelope(const OGREnvelope * envelope)
{
	double height = envelope->MaxY - envelope->MinY;
	double width = envelope->MaxX - envelope->MinX;

	_scale = _tgis_max(abs(height / _surfHeight), abs(width / _surfWidth));

	SetSpatialCenter((envelope->MaxX + envelope->MinX) / 2.0, (envelope->MaxY + envelope->MinY) / 2.0);
}

void GeoViewPort::IncludeEnvelope(double spatialLeft, double spatialTop, double spatialRight, double spatialBottom)
{
	double height = spatialRight - spatialLeft;
	double width = spatialBottom - spatialTop;

	_scale = _tgis_max(abs(height / _surfHeight), abs(width / _surfWidth));

	SetSpatialCenter((spatialRight + spatialLeft) / 2.0, (spatialBottom + spatialTop) / 2.0);
}

void GeoViewPort::GetEnvelope(OGREnvelope * extent) const
{
	extent->MinX = _spatialLeft;
	extent->MaxY = _spatialTop;
	extent->MaxX = _spatialRight;
	extent->MinY = _spatialBottom;
}

void GeoViewPort::GetEnvelope(double * spatialLeft, double * spatialTop, double * spatialRight, double * spatialBottom) const
{
	*spatialLeft = _spatialLeft;
	*spatialTop = _spatialTop;
	*spatialRight = _spatialRight;
	*spatialBottom = _spatialBottom;
}

void GeoViewPort::GetViewScale(double * scale) const
{
	*scale = _scale;
}

void GeoViewPort::SetViewScale(double scale)
{
	_scale = scale;

	UpdateViewPort();
}

void GeoViewPort::UpdateViewPort()
{
	double halfW = _scale*_surfWidth / 2.0;
	double halfH = _scale*_surfHeight / 2.0;
	_spatialLeft = _spatialCenterX - halfW;
	_spatialTop = _spatialCenterY + halfH;
	_spatialRight = _spatialCenterX + halfW;
	_spatialBottom = _spatialCenterY - halfH;
}

void GeoViewPort::Surface2Spatial(int surfX, int surfY, double * spatialX, double * spatialY) const
{
	*spatialX = _spatialLeft + surfX*_scale;
	*spatialY = _spatialTop - surfY*_scale;
}

void GeoViewPort::Spatial2Surface(double spatialX, double spatialY, double * surfX, double * surfY) const
{
	*surfX = (spatialX - _spatialLeft) / _scale;
	*surfY = (_spatialTop - spatialY) / _scale;
}

void GeoViewPort::Spatial2Surface(double spatialX, double spatialY, int * surfX, int * surfY) const
{
	*surfX = (int)_tgis_round((spatialX - _spatialLeft) / _scale, 0);
	*surfY = (int)_tgis_round((_spatialTop - spatialY) / _scale, 0);
}

END_NAME_SPACE(tGis, Core)
