#pragma once

#ifndef __I_SCREENLAYER_H__
#define __I_SCREENLAYER_H__

#include "Helper.h"

#include "ogr_spatialref.h"

class OGREnvelope;
class OGRSpatialReference;

BEGIN_NAME_SPACE(tGis, Core)

struct IGeoSurface;

struct TGISCORE_API IScreenLayer
{
	virtual const OGREnvelope* GetEnvelope() = 0;
	virtual const OGRSpatialReference* GetSpatialReference() = 0;
	virtual bool GetVisible() = 0;
	virtual void SetVisible(bool) = 0;
	virtual float GetOpacity() = 0;
	virtual void SetOpacity(float) = 0;

	virtual void Paint(IGeoSurface*) = 0;

	virtual ~IScreenLayer() {};
private:
	IScreenLayer(const IScreenLayer &) = delete;
	IScreenLayer &operator=(const IScreenLayer &) = delete;
};


END_NAME_SPACE(tGis, Core)


#endif
