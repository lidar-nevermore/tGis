#pragma once

#ifndef __I_GEOSURFACE_H__
#define __I_GEOSURFACE_H__

#include "Helper.h"
#include "tGisCoreCfg.h"

#include "ISurface.h"
#include "GeoViewPort.h"

class OGREnvelope;
class OGRSpatialReference;

BEGIN_NAME_SPACE(tGis, Core)

struct TGIS_CORE_API IGeoSurface : public ISurface
{
	virtual const GeoViewPort* GetViewPort() const = 0;

protected:
	IGeoSurface() {};
	virtual ~IGeoSurface() {};
private:
	IGeoSurface(const IGeoSurface &) = delete;
	IGeoSurface &operator=(const IGeoSurface &) = delete;
};

typedef IGeoSurface* IGeoSurfacePtr;

END_NAME_SPACE(tGis, Core)


#endif
