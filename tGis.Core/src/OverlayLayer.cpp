#include "OverlayLayer.h"

BEGIN_NAME_SPACE(tGis, Core)

class OverlayLayerImpl
{
public:
	OverlayLayerImpl(OverlayLayer* owner)
	{
		_owner = owner;
	}

	OverlayLayer* _owner;

	vector<IOverlayObject*> _vecOverlayObject;
	bool _visible;
};

OverlayLayer::OverlayLayer()
{
	_impl_ = new OverlayLayerImpl(this);
	_impl_->_visible = true;
}

OverlayLayer::~OverlayLayer()
{
	for (vector<IOverlayObject*>::iterator it = _impl_->_vecOverlayObject.begin(); it != _impl_->_vecOverlayObject.end(); ++it)
	{
		IOverlayObject* o = *it;
		if (o->_is_in_heap)
			delete o;
	}
	_impl_->_vecOverlayObject.clear();
	delete _impl_;
}

bool OverlayLayer::GetVisible()
{
	return _impl_->_visible;
}

void OverlayLayer::SetVisible(bool v)
{
	_impl_->_visible = v;
}

void OverlayLayer::AddOverlayObject(IOverlayObject * o)
{
	_impl_->_vecOverlayObject.push_back(o);
}

void OverlayLayer::RemoveOverlayObject(IOverlayObject * o)
{
	for (vector<IOverlayObject*>::iterator it = _impl_->_vecOverlayObject.begin(); it != _impl_->_vecOverlayObject.end(); ++it)
	{
		if (*it == o)
		{
			_impl_->_vecOverlayObject.erase(it);
			break;
		}
	}
}

void OverlayLayer::Paint(IGeoSurface * surf)
{
	for (vector<IOverlayObject*>::reverse_iterator it = _impl_->_vecOverlayObject.rbegin(); it != _impl_->_vecOverlayObject.rend(); ++it)
	{
		if((*it)->GetVisible())
			(*it)->Paint(surf);
	}
}

END_NAME_SPACE(tGis, Core)