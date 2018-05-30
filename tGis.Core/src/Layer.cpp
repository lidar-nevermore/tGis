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

IMap * Layer::GetMap()
{
	return _map;
}

void Layer::SetMap(IMap * map)
{
	_map = map;
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
	if (_alpha > 255)
		_alpha = 255;
}



END_NAME_SPACE(tGis, Core)


