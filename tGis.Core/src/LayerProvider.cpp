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

void LayerProvider::SetCreationUI(const CreationUI ui)
{
	_uiCreation = ui;
}

ILayer * LayerProvider::UI_CreateLayer(IDataset * dataset)
{
	assert(_uiCreation != nullptr);

	return _uiCreation(this, dataset);
}

void LayerProvider::SetPropertyUI(const PropertyUI ui)
{
	_uiProperty = ui;
}

void LayerProvider::UI_LayerProperty(ILayer * layer)
{
	assert(_uiProperty != nullptr);

	_uiProperty(this, layer);
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


