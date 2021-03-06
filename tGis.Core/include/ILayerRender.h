#pragma once

#ifndef __I_LAYERRENDER_H__
#define __I_LAYERRENDER_H__

#include "Helper.h"
#include "ITypedObject.h"
#include "IInclusionObject.h"

class OGREnvelope;
class OGRSpatialReference;

BEGIN_NAME_SPACE(tGis, Core)

struct IDataset;
struct IGeoSurface;

struct TGIS_CORE_API ILayerRender : public ITypedObject, public IInclusionObject
{
	friend class Layer;

	//所关联的Layer的数据在Layer所属Map的投影下的范围
	virtual const OGREnvelope* GetEnvelope() = 0;
	virtual const OGRSpatialReference* GetSpatialReference() = 0;
	virtual bool CanTransformTo(const OGRSpatialReference*) = 0;

	virtual float GetOpacity() = 0;
	virtual void SetOpacity(float) = 0;
	virtual void Paint(IGeoSurface*) = 0;

	static bool CanTransform(const OGRSpatialReference * from, const OGRSpatialReference * to);

	ILayerRender() {};
	virtual ~ILayerRender() {};
private:
	ILayerRender(const ILayerRender &) = delete;
	ILayerRender &operator=(const ILayerRender &) = delete;
};

typedef ILayerRender* ILayerRenderPtr;

END_NAME_SPACE(tGis, Core)


#endif
