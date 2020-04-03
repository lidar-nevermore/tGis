#pragma once

#ifndef __I_LAYER_H__
#define __I_LAYER_H__

#include "Helper.h"
#include "tGisCoreCfg.h"
#include "ITGisObject.h"

class OGREnvelope;
class OGRSpatialReference;

BEGIN_NAME_SPACE(tGis, Core)

struct IDataset;
struct IGeoSurface;
struct IMap;
struct ILayerRender;

struct TGIS_CORE_API ILayer : public ITGisObject
{
	friend class Map;

	virtual const char* GetName() = 0;
	virtual void SetName(const char*) = 0;
	//Layer的数据在Layer所属Map的投影下的范围
	virtual const OGREnvelope* GetEnvelope() = 0;

	virtual IDataset* GetDataset() = 0;
	virtual IMap* GetMap() = 0;
	virtual void SetMap(IMap*) = 0;
	virtual ILayerRender* GetRender() = 0;
	virtual void SetRender(ILayerRender* render) = 0;

	virtual bool GetVisible() = 0;
	virtual void SetVisible(bool) = 0;

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
