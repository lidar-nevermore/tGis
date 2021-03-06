#pragma once

#ifndef __LAYER_H__
#define __LAYER_H__

#include "Helper.h"

#include "ILayer.h"
#include "ILayerRender.h"
#include "IMap.h"

class OGREnvelope;
class OGRSpatialReference;

BEGIN_NAME_SPACE(tGis, Core)

struct IDataset;
class LayerImpl;

class TGIS_CORE_API Layer : public ILayer
{
	friend class Map;
public:
	Layer(IDataset* dt);
	virtual ~Layer();

public:
	virtual const char* GetName();

	virtual void SetName(const char* name);

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
		if (_render == render)
			return;
		if (_render != nullptr && _render->_is_in_heap)
			delete _render;
		_render = render;
	}

	virtual bool GetVisible()
	{
		return _visible;
	}

	virtual void SetVisible(bool visible)
	{
		bool _fireEvent = _visible != visible;
		_visible = visible;
		if (_fireEvent && _map != nullptr)
			_map->LayerVisibleChangedEvent(_map, this);
	}

	virtual void Paint(IGeoSurface* surf)
	{
		if (_visible == false)
			return;

		if (_render != nullptr)
			_render->Paint(surf);
	}


protected:
	bool _visible;
	IMap* _map;
	ILayerRender* _render;
	IDataset* _dataset;

private:
	LayerImpl* _impl_;
};

END_NAME_SPACE(tGis, Core)

#endif
