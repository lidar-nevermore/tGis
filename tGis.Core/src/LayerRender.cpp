#include "LayerRender.h"
#include "IDataset.h"

BEGIN_NAME_SPACE(tGis, Core)

LayerRender::LayerRender(ILayer* layer)
{
	_opacity = 1.0;
	_alpha = 255;
	_layer = layer;
}

LayerRender::~LayerRender()
{
}

float LayerRender::GetOpacity()
{
	return _opacity;
}

void LayerRender::SetOpacity(float opacity)
{
	_opacity = opacity;
	_alpha = unsigned char(255 * _opacity);
	if (_alpha > 255)
		_alpha = 255;
}



END_NAME_SPACE(tGis, Core)


