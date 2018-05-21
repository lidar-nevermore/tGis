#include "Layer.h"
#include "IDataset.h"

BEGIN_NAME_SPACE(tGis, Core)

Layer::Layer()
{
	_visible = true;
	_opacity = 1.0;
	_alpha = 255;
}

Layer::~Layer()
{
}

const char * Layer::GetName()
{
	return _name.c_str();
}

void Layer::SetName(const char * name)
{
	_name = name;
}

bool Layer::GetVisible()
{
	return _visible;
}

void Layer::SetVisible(bool visible)
{
	_visible = visible;
}

float Layer::GetOpacity()
{
	return _opacity;
}

void Layer::SetOpacity(float opacity)
{
	_opacity = opacity;
	_alpha = unsigned char(255 * _opacity);
}



END_NAME_SPACE(tGis, Core)


