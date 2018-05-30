#include "MyMap.h"

#include "tOrganizer.h"

using namespace tGis::Core;

MyMap::MyMap()
{
}


MyMap::~MyMap()
{
	for (vector<ILayer*>::iterator it = _vecLayer.begin(); it != _vecLayer.end(); it++)
	{
		ILayer* layer = *it;
		ILayerProvider* provider = LayerProviderRepository::INSTANCE().GetLayerProvider(layer->GetType());
		provider->ReleaseLayer(layer);
	}
}

void MyMap::ClearLayers(LayerFunc func)
{
	for (vector<ILayer*>::iterator it = _vecLayer.begin(); it != _vecLayer.end(); it++)
	{
		ILayer* layer = *it;
		ILayerProvider* provider = LayerProviderRepository::INSTANCE().GetLayerProvider(layer->GetType());
		provider->ReleaseLayer(layer);
	}
	_vecLayer.clear();
}
