#pragma once

#ifndef __LAYER_H__
#define __LAYER_H__

#include "Helper.h"

#include "ILayer.h"
#include "ILayerRender.h"

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
	Layer(IDataset* dt);
	virtual ~Layer();

public:
	virtual const char* GetName()
	{
		return _name.c_str();
	}

	virtual void SetName(const char* name)
	{
		_name = name;
	}

	virtual const OGREnvelope* GetEnvelope()
	{
		if (_render != nullptr)
			return _render->GetEnvelope();
		return nullptr;
	}

	virtual IDataset* GetDataset()
	{
		return _dataset;
	}

	virtual IMap* GetMap()
	{
		return _map;
	}

	virtual void SetMap(IMap* map)
	{
		_map = map;
	}

	virtual ILayerRender* GetRender()
	{
		return _render;
	}

	virtual void SetRender(ILayerRender* render)
	{
		_render = render;
	}

	virtual bool GetVisible()
	{
		return _visible;
	}

	virtual void SetVisible(bool visible)
	{
		_visible = visible;
	}

	virtual void Paint(IGeoSurface* surf)
	{
		if (_visible == false)
			return;

		if (_render != nullptr)
			_render->Paint(surf);
	}


protected:
	string _name;
	bool _visible;
	IMap* _map;
	ILayerRender* _render;
	IDataset* _dataset;
};

END_NAME_SPACE(tGis, Core)

#endif
