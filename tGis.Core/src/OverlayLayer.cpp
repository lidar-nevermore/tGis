#include "OverlayLayer.h"

BEGIN_NAME_SPACE(tGis, Core)

OverlayLayer::OverlayLayer()
{
	_visible = true;
}


OverlayLayer::~OverlayLayer()
{
}

bool OverlayLayer::GetVisible()
{
	return _visible;
}

void OverlayLayer::SetVisible(bool v)
{
	_visible = v;
}

void OverlayLayer::AddOverlayObject(IOverlayObject * o)
{
	_vecOverlayObject.push_back(o);
}

void OverlayLayer::RemoveOverlayObject(IOverlayObject * o)
{
	for (vector<IOverlayObject*>::iterator it = _vecOverlayObject.begin(); it != _vecOverlayObject.end(); ++it)
	{
		if (*it == o)
		{
			_vecOverlayObject.erase(it);
			break;
		}
	}
}

void OverlayLayer::Paint(IGeoSurface * surf)
{
	for (vector<IOverlayObject*>::reverse_iterator it = _vecOverlayObject.rbegin(); it != _vecOverlayObject.rend(); ++it)
	{
		(*it)->Paint(surf);
	}
}

END_NAME_SPACE(tGis, Core)