#include "OverlayPolygon.h"
#include "SimpleLineSymbol.h"
#include "SimpleFillSymbol.h"
#include <vector>
#include "IGeoSurface.h"

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

static SimpleLineSymbol g_LineSymbol(255, 0, 0, 255, 1, SimpleLineSymbol::Solid);

class OverlayPolygonImpl
{
public:
	OverlayPolygonImpl(OverlayPolygon* owner)
	{
		_owner = owner;
	}

	OverlayPolygon* _owner;
	vector<pair<double,double>> _vecVertex;
};

OverlayPolygon::OverlayPolygon()
{
	_impl_ = new OverlayPolygonImpl(this);
	_lineSymbol = &g_LineSymbol;
	_fillSymbol = nullptr;
	_visible = true;
}

OverlayPolygon::OverlayPolygon(double* x, double* y, int size)
	: OverlayPolygon()
{
	for (int i = 0; i < size; i++)
		_impl_->_vecVertex.push_back(std::make_pair(x[i], y[i]));
}

OverlayPolygon::~OverlayPolygon()
{
	delete _impl_;
}

bool OverlayPolygon::GetVisible()
{
	return _visible;
}

void OverlayPolygon::SetVisible(bool visible)
{
	_visible = visible;
}

void OverlayPolygon::Paint(IGeoSurface * surf)
{
	const GeoViewPort* viewPort = surf->GetViewPort();
	if (_lineSymbol != nullptr)
	{
		_lineSymbol->BeginPaint((ISurface*)surf);
		for (vector<pair<double, double>>::iterator it = _impl_->_vecVertex.begin(); it != _impl_->_vecVertex.end(); ++it)
		{
			pair<double, double>& vertex = *it;
			int x, y;
			viewPort->Spatial2Surface(vertex.first, vertex.second, &x, &y);
			_lineSymbol->AppendVertex(x, y);
		}
		_lineSymbol->EndPaint((ISurface*)surf);
	}
	if (_fillSymbol != nullptr)
	{
		_fillSymbol->BeginPaint((ISurface*)surf);
		_fillSymbol->BeginContour(surf);
		for (vector<pair<double, double>>::iterator it = _impl_->_vecVertex.begin(); it != _impl_->_vecVertex.end(); ++it)
		{
			pair<double, double>& vertex = *it;
			int x, y;
			viewPort->Spatial2Surface(vertex.first, vertex.second, &x, &y);
			_fillSymbol->AppendVertex(x, y);
		}
		_fillSymbol->EndContour(surf);
		_fillSymbol->EndPaint((ISurface*)surf);
	}
}

void OverlayPolygon::SetLineSymbol(ILineSymbol * lineSymbol)
{
	_lineSymbol = lineSymbol;
}

void OverlayPolygon::SetFillSymbol(IFillSymbol * fillSymbol)
{
	_fillSymbol = fillSymbol;
}

size_t OverlayPolygon::GetVertexCount()
{
	return _impl_->_vecVertex.size();
}

void OverlayPolygon::GetVertex(size_t pos, double * x, double * y)
{
	pair<double, double>& vertex = _impl_->_vecVertex.at(pos);
	*x = vertex.first;
	*y = vertex.second;
}

void OverlayPolygon::RemoveVertex(size_t pos)
{
	vector<pair<double,double>>::iterator it = _impl_->_vecVertex.begin() + pos;
	_impl_->_vecVertex.erase(it);
}


END_NAME_SPACE(tGis, Core)
