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
		ILayerProvider* provider = layer->GetProvider();
		provider->ReleaseLayer(layer);
	}
	_vecLayer.clear();
}

void MyMap::ClearLayers(LayerFunc func)
{
	for (vector<ILayer*>::iterator it = _vecLayer.begin(); it != _vecLayer.end(); it++)
	{
		ILayer* layer = *it;
		ILayerProvider* provider = layer->GetProvider();
		provider->ReleaseLayer(layer);
	}
	_vecLayer.clear();
}
