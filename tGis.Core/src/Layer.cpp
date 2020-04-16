#include "Layer.h"
#include "IDataset.h"

#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)


class LayerImpl
{
public:
	LayerImpl(Layer* owner)
	{
		_owner = owner;
	}

	Layer* _owner;
	string _name;
};


Layer::Layer(IDataset* dt)
{
	_impl_ = new LayerImpl(this);
	_visible = true;
	_map = nullptr;
	_render = nullptr;
	_dataset = dt;
	_impl_->_name = dt->GetName();
}

Layer::~Layer()
{
	if (_render != nullptr && _render->_is_in_heap)
		delete _render;

	delete _impl_;
}

const char * Layer::GetName()
{
	return _impl_->_name.c_str();
}

void Layer::SetName(const char * name)
{
	_impl_->_name = name;
}

END_NAME_SPACE(tGis, Core)

