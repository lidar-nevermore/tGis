#include "LayerProvider.h"
#include "ILayer.h"

#include <cassert>

BEGIN_NAME_SPACE(tGis, Core)

LayerProvider::LayerProvider()
{
}


LayerProvider::~LayerProvider()
{

}

ILayer * LayerProvider::CreateLayer(IDataset * dataset, const char * creationString)
{
	return nullptr;
}

void LayerProvider::ReleaseLayer(ILayer * layer)
{
	delete layer;
}

void LayerProvider::Release()
{
	delete this;
}

END_NAME_SPACE(tGis, Core)


