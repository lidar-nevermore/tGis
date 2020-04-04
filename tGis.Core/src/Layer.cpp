#include "Layer.h"
#include "IDataset.h"

BEGIN_NAME_SPACE(tGis, Core)

const char* const Layer::_type = "00FBBC87-473A-4484-9C13-EAD0A565F7D7";


const char * Layer::GetType()
{
	return _type;
}

bool Layer::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return false;
}

const char * Layer::S_GetType()
{
	return _type;
}

Layer::Layer(IDataset* dt)
{
	_visible = true;
	_map = nullptr;
	_render = nullptr;
	_dataset = dt;
}

Layer::~Layer()
{
	if (_render != nullptr && _render->_is_in_heap)
		delete _render;
}

END_NAME_SPACE(tGis, Core)

