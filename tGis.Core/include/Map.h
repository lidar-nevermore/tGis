#pragma once

#ifndef __MAP_H__
#define __MAP_H__

#include "Helper.h"
#include "IMap.h"

#include "gdal.h"


BEGIN_NAME_SPACE(tGis, Core)

class MapImpl;

class TGIS_CORE_API Map : public IMap
{
public:
	Map();
	virtual ~Map();

private:
	Map(const Map &) = delete;
	Map &operator=(const Map &) = delete;

public:
	virtual const char* GetName();
	virtual void SetName(const char*);
	virtual const OGREnvelope* GetEnvelope();
	virtual const OGRSpatialReference* GetSpatialReference();

	virtual size_t GetLayerCount();
	virtual ILayer* GetLayer(size_t);
	virtual bool AddLayer(ILayer*);
	virtual ILayer* RemoveLayer(size_t);
	virtual void RemoveLayer(ILayer*);
	virtual void RemoveLayer(IDataset*);
	virtual bool InsertLayer(size_t, ILayer*);
	virtual void MoveLayer(size_t from, size_t to);
	virtual void ClearLayers();
	virtual void Paint(IGeoSurface*);

protected:
	OGREnvelope _envelope;
	OGRSpatialReference* _spatialRef;

protected:
	void UpdateEnvelope();
	void MergeEnvelope(const OGRSpatialReference* spatialRef,const OGREnvelope *envelope);

private:
	MapImpl* _impl_;
};

END_NAME_SPACE(tGis, Core)


#endif
