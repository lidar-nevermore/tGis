#include "OverlayRect.h"
#include "SimpleLineSymbol.h"
#include "SimpleFillSymbol.h"

BEGIN_NAME_SPACE(tGis, Core)

static SimpleLineSymbol g_LineSymbol(SimpleLineSymbol::Solid);

OverlayRect::OverlayRect()
{
	g_LineSymbol.SetColor(255, 0, 0, 255);
	_left = 0;
	_top = 0;
	_right = 0;
	_bottom = 0;
	_lineSymbol = &g_LineSymbol;
	_fillSymbol = nullptr;
	_visible = true;
}

OverlayRect::OverlayRect(int left, int top, int right, int bottom)
{
	_left = left;
	_top = top;
	_right = right;
	_bottom = bottom;
	_lineSymbol = &g_LineSymbol;
	_fillSymbol = nullptr;
	_visible = true;
}

OverlayRect::~OverlayRect()
{
}

bool OverlayRect::GetVisible()
{
	return _visible;
}

void OverlayRect::SetVisible(bool visible)
{
	_visible = visible;
}

void OverlayRect::Paint(IGeoSurface * surf)
{
	int x[5] = { _left,_right,_right,_left,_left };
	int y[5] = { _top,_top,_bottom,_bottom,_top };
	if (_lineSymbol != nullptr)
	{
		_lineSymbol->Paint((ISurface*)surf, 5, x, y, nullptr, nullptr);
	}
	if (_fillSymbol != nullptr)
	{
		_fillSymbol->Paint((ISurface*)surf, 5, x, y, nullptr, nullptr);
	}
}

void OverlayRect::SetLineSymbol(ISymbol * lineSymbol)
{
	_lineSymbol = lineSymbol;
}

void OverlayRect::SetFillSymbol(ISymbol * fillSymbol)
{
	_fillSymbol = fillSymbol;
}


END_NAME_SPACE(tGis, Core)
