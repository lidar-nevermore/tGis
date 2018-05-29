#pragma once

#ifndef __I_LAYER_H__
#define __I_LAYER_H__

#include "Helper.h"

class OGREnvelope;
class OGRSpatialReference;

BEGIN_NAME_SPACE(tGis, Core)

struct IDataset;
struct IGeoSurface;

struct TGIS_API ILayer
{
	virtual const char* GetType() = 0;
	virtual const char* GetName() = 0;
	virtual const char* GetCreationString() = 0;
	virtual void SetName(const char*) = 0;
	virtual const OGREnvelope* GetEnvelope() = 0;
	virtual const OGRSpatialReference* GetSpatialReference() = 0;
	virtual IDataset* GetDataset() = 0;
	virtual bool GetVisible() = 0;
	virtual void SetVisible(bool) = 0;
	virtual float GetOpacity() = 0;
	virtual void SetOpacity(float) = 0;
	virtual void Paint(IGeoSurface*) = 0;

	ILayer() {};
	virtual ~ILayer() {};
private:
	ILayer(const ILayer &) = delete;
	ILayer &operator=(const ILayer &) = delete;
};

typedef ILayer* ILayerPtr;

END_NAME_SPACE(tGis, Core)


#endif
