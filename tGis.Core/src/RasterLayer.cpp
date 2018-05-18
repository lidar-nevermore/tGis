#include "RasterLayer.h"
#include "MyGDALRasterDataset.h"
#include "IGeoSurface.h"
#include "gdal.h"
#include "gdal_priv.h"
#include "ogr_spatialref.h"
#include <algorithm>
#include <cmath>

BEGIN_NAME_SPACE(tGis, Core)


RasterLayer::RasterLayer()
{
	_visible = true;
	_opacity = 1.0;
	_alpha = 255;
}

RasterLayer::RasterLayer(MyGDALRasterDataset* dataset)
{
	if (!dataset->IsNorthUp())
	{
		throw std::exception("不支持非北方朝上的影响！");
	}
	_dataset = dataset;
	_visible = true;
	_opacity = 1.0;
	_alpha = 255;
}


RasterLayer::~RasterLayer()
{
}

void RasterLayer::SetDataset(MyGDALRasterDataset * dataset)
{
	if (!dataset->IsNorthUp())
	{
		throw std::exception("不支持非北方朝上的影响！");
	}
	_dataset = dataset;
}


const char * RasterLayer::GetName()
{
	return _name.c_str();
}

void RasterLayer::SetName(const char * name)
{
	_name = name;
}

const OGREnvelope * RasterLayer::GetEnvelope()
{
	return _dataset->GetEnvelope();
}

const OGRSpatialReference * RasterLayer::GetSpatialReference()
{
	return _dataset->GetSpatialReference();
}

bool RasterLayer::CanTransformTo(const OGRSpatialReference* spatialRef)
{
	const OGRSpatialReference * thisSpatialRef = _dataset->GetSpatialReference();
	if (thisSpatialRef == spatialRef)
		return true;
	
	if (thisSpatialRef != nullptr)
		return thisSpatialRef->IsSame(spatialRef);

	return false;
}

bool RasterLayer::GetVisible()
{
	return _visible;
}

void RasterLayer::SetVisible(bool visible)
{
	_visible = visible;
}

float RasterLayer::GetOpacity()
{
	return _opacity;
}

void RasterLayer::SetOpacity(float opacity)
{
	_opacity = opacity;
	_alpha = unsigned char(255 * _opacity);
}

IDataset * RasterLayer::GetDataset(int)
{
	return _dataset;
}

bool RasterLayer::PreparePaint(IGeoSurface* surf)
{
	if (_visible == false)
		return false;
	if (_alpha == 0)
		return false;

	OGREnvelope aoienvelope = *(_dataset->GetEnvelope());
	aoienvelope.Intersect(*(surf->GetEnvelope()));
	if (!aoienvelope.IsInit())
		return false;

	double minInPixX = 0;
	double minInPixY = 0;
	double maxInPixX = 0;
	double maxInPixY = 0;

	_dataset->Spatial2Pixel(aoienvelope.MinX, aoienvelope.MinY, &minInPixX, &minInPixY);
	_dataset->Spatial2Pixel(aoienvelope.MaxX, aoienvelope.MaxY, &maxInPixX, &maxInPixY);


	_minPixX = min(minInPixX, maxInPixX);
	_minPixY = min(minInPixY, maxInPixY);
	_maxPixX = max(minInPixX, maxInPixX);
	_maxPixY = max(minInPixY, maxInPixY);

	double minInSurfX = 0;
	double minInSurfY = 0;
	double maxInSurfX = 0;
	double maxInSurfY = 0;

	surf->Spatial2Surface(aoienvelope.MinX, aoienvelope.MinY, &minInSurfX, &minInSurfY);
	surf->Spatial2Surface(aoienvelope.MaxX, aoienvelope.MaxY, &maxInSurfX, &maxInSurfY);

	_minSurfX = min(minInSurfX, maxInSurfX);
	_minSurfY = min(minInSurfY, maxInSurfY);
	_maxSurfX = max(minInSurfX, maxInSurfX);
	_maxSurfY = max(minInSurfY, maxInSurfY);

	_surfPixRatio = (_maxPixX - _minPixX) / (_maxSurfX - _minSurfX);
	_outerResample = _surfPixRatio < 0.1;

	return true;
}


END_NAME_SPACE(tGis, Core)
