#include "Layer.h"
#include "IDataset.h"

BEGIN_NAME_SPACE(tGis, Core)

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

