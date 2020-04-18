#pragma once

#ifndef __I_MAP_H__
#define __I_MAP_H__

#include "Helper.h"
#include "ILayer.h"
#include "Event.h"

BEGIN_NAME_SPACE(tGis, Core)

struct IDataset;
struct IGeoSurface;
struct IMap;

template struct TGIS_CORE_API IEventHandler<IMap*>;
template class TGIS_CORE_API Event<IMap*>;

template struct TGIS_CORE_API IEventHandler<IMap*, ILayer*, size_t>;
template class TGIS_CORE_API Event<IMap*, ILayer*, size_t>;

template struct TGIS_CORE_API IEventHandler<IMap*, ILayer*, size_t, ILayer*, size_t>;
template class TGIS_CORE_API Event<IMap*, ILayer*, size_t, ILayer*, size_t>;

struct TGIS_CORE_API IMap
{
	virtual const char* GetName() = 0;
	virtual void SetName(const char*) = 0;
	virtual const OGREnvelope* GetEnvelope() = 0;
	virtual const OGRSpatialReference* GetSpatialReference() = 0;

	//图层在Map中是有序的，最上层的序号为0，从上至下依次递增
	//绘制时上层覆盖下层，也就是序号为0的层显示在最上面
	//Map对象不支持多线程访问，
	virtual size_t GetLayerCount() = 0;
	virtual ILayer* GetLayer(size_t) = 0;
	//返回值大于等于GetLayerCount表示不存在
	virtual size_t GetLayerPos(ILayer*) = 0;
	virtual bool AddLayer(ILayer*) = 0;
	virtual bool InsertLayer(size_t, ILayer*) = 0;
	virtual ILayer* RemoveLayer(size_t) = 0;
	virtual void RemoveLayer(ILayer*) = 0;
	virtual void RemoveLayer(IDataset*) = 0;
	virtual void MoveLayer(size_t from, size_t to) = 0;
	virtual void ClearLayers() = 0;

	virtual void Paint(IGeoSurface*) = 0;

	Event<IMap*, ILayer*, size_t> LayerAddedEvent;
	Event<IMap*, ILayer*, size_t> LayerRemovedEvent;
	Event<IMap*, ILayer*, size_t, ILayer*, size_t> LayerMovedEvent;
	Event<IMap*> LayerClearedEvent;

	IMap() {};
	virtual ~IMap() {};
private:
	IMap(const IMap &) = delete;
	IMap &operator=(const IMap &) = delete;
};

typedef IMap* IMapPtr;

END_NAME_SPACE(tGis, Core)


#endif


