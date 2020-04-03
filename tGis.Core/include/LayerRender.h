#pragma once

#ifndef __LAYERRENDER_H__
#define __LAYERRENDER_H__

#include "Helper.h"

#include "ILayerRender.h"

#include <string>

using namespace std;

class OGREnvelope;
class OGRSpatialReference;

BEGIN_NAME_SPACE(tGis, Core)

struct IDataset;
struct ILayer;

class TGIS_CORE_API LayerRender : public ILayerRender
{
	friend class Map;
public:
	LayerRender(ILayer* layer);
	virtual ~LayerRender();

public:
	virtual float GetOpacity();
	virtual void SetOpacity(float);

protected:
	float _opacity;
	unsigned char _alpha;
	ILayer* _layer;
};

END_NAME_SPACE(tGis, Core)

#endif
