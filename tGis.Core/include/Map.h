#pragma once

#ifndef __MAP_H__
#define __MAP_H__

#include "Helper.h"

#include "IMap.h"


#include <string>
#include <vector>

#include "gdal.h"

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

class TGIS_API Map : public IMap
{
public:
	Map();
	virtual ~Map();

public:
	virtual const char* GetName();
	virtual void SetName(const char*);
	virtual const OGREnvelope* GetEnvelope();
	virtual const OGRSpatialReference* GetSpatialReference();
	virtual bool CanTransformFrom(const OGRSpatialReference*);
	virtual size_t GetLayerCount();
	virtual ILayer* GetLayer(size_t);
	virtual size_t GetLayerIndex(ILayer*);
	virtual size_t AddLayer(ILayer*, bool* added = nullptr);
	virtual ILayer* RemoveLayer(size_t);
	virtual void RemoveLayer(ILayer*);
	virtual void RemoveLayer(IDataset*);
	virtual bool InsertLayer(size_t, ILayer*);
	virtual void MoveLayer(size_t from, size_t to);
	virtual void ClearLayers(LayerFunc func = nullptr);
	virtual void Paint(IGeoSurface*);

protected:
	vector<ILayer*> _vecLayer;
	string _name;
	OGREnvelope _envelope;
	OGRSpatialReference* _spatialRef;

protected:
	void MergeEnvelope();
	void MergeEnvelope(const OGRSpatialReference* spatialRef,const OGREnvelope *envelope);

private:
	Map(const Map &) = delete;
	Map &operator=(const Map &) = delete;
};

END_NAME_SPACE(tGis, Core)


#endif
