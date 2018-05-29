#pragma once

#ifndef __I_MAP_H__
#define __I_MAP_H__

#include "Helper.h"
#include "ILayer.h"

BEGIN_NAME_SPACE(tGis, Core)

struct IDataset;
struct IGeoSurface;

struct TGIS_API IMap
{
	virtual const char* GetName() = 0;
	virtual void SetName(const char*) = 0;
	virtual const OGREnvelope* GetEnvelope() = 0;
	virtual const OGRSpatialReference* GetSpatialReference() = 0;
	virtual bool CanTransformFrom(const OGRSpatialReference*) = 0;

	virtual int GetLayerCount() = 0;
	virtual ILayer* GetLayer(int) = 0;
	virtual bool AddLayer(ILayer*) = 0;
	virtual bool InsertLayer(int, ILayer*) = 0;
	virtual void RemoveLayer(int) = 0;
	virtual void RemoveLayer(ILayer*) = 0;


	virtual void Paint(IGeoSurface*) = 0;

	IMap() {};
	virtual ~IMap() {};
private:
	IMap(const IMap &) = delete;
	IMap &operator=(const IMap &) = delete;
};

typedef IMap* IMapPtr;

END_NAME_SPACE(tGis, Core)


#endif


