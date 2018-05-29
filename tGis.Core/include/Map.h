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
	virtual int GetLayerCount();
	virtual ILayer* GetLayer(int);
	virtual bool AddLayer(ILayer*);
	virtual void RemoveLayer(int);
	virtual void RemoveLayer(ILayer*);
	virtual bool InsertLayer(int, ILayer*);

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
