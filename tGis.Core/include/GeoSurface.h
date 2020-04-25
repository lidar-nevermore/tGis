#pragma once

#ifndef __GEOSURFACE_H__
#define __GEOSURFACE_H__

#include "Helper.h"

#include "IGeoSurface.h"

#include "gdal.h"

BEGIN_NAME_SPACE(tGis, Core)

class TGIS_CORE_API GeoSurface : public IGeoSurface
{
	friend class MapWidget;
protected:
	GeoSurface();
	virtual ~GeoSurface();

public:
	virtual inline const GeoViewPort* GetViewPort() const
	{
		return &_viewPort;
	}

	virtual inline void GetSize(int* surfW, int* surfH) const
	{
		*surfW = _viewPort._surfWidth;
		*surfH = _viewPort._surfHeight;
	}

	void inline Surface2glndc(int surfX, int surfY, GLfloat* ndcX, GLfloat* ndcY) override
	{
		int surfWidth = _viewPort._surfWidth;
		int surfHeight = _viewPort._surfHeight;
		*ndcX = (2.0f*surfX) / surfWidth - 1.0f;
		*ndcY = 1.0f - (2.0f*surfY) / surfHeight;
	}

	virtual void BeginPaint(bool isCache);

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
