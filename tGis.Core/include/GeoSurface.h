#pragma once

#ifndef __GEOSURFACE_H__
#define __GEOSURFACE_H__

#include "Helper.h"

#include "IGeoSurface.h"

#include "gdal.h"

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)
class TGIS_CORE_API GeoSurface : public IGeoSurface
{
	friend class MapWidget;
protected:
	GeoSurface();
	virtual ~GeoSurface();

public:
	virtual const GeoViewPort* GetViewPort() const
	{
		return &_viewPort;
	}

protected:
	virtual void SetViewPort(GeoViewPort* viewPort)
	{
		_viewPort = *viewPort;
	}

protected:
	GeoViewPort _viewPort;
};

END_NAME_SPACE(tGis, Core)


#endif
