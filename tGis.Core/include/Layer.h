#pragma once

#ifndef __LAYER_H__
#define __LAYER_H__

#include "Helper.h"

#include "ILayer.h"

#include <string>

using namespace std;

class OGREnvelope;
class OGRSpatialReference;

BEGIN_NAME_SPACE(tGis, Core)

struct IDataset;

class TGIS_CORE_API Layer : public ILayer
{
	friend class Map;
public:
	Layer(ILayerProvider* provider);
	virtual ~Layer();

public:
	virtual ILayerProvider* GetProvider();
	virtual ILayer* Clone(IDataset*);
	virtual const char* GetName();
	virtual void SetName(const char* name);
	virtual IMap* GetMap();
	virtual void SetMap(IMap*);
	virtual bool GetVisible();
	virtual void SetVisible(bool);
	virtual float GetOpacity();
	virtual void SetOpacity(float);

protected:
	string _name;
	bool _visible;
	float _opacity;
	unsigned char _alpha;
	IMap* _map;
	ILayerProvider* _provider;
};

END_NAME_SPACE(tGis, Core)

#endif
