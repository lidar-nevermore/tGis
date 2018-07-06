#include "SimpleMarkerSymbol.h"
#include "SimpleSymbolLibrary.h"

#include "ISurface.h"
#include "SimpleLineSymbol.h"
#include "SimpleFillSymbol.h"


BEGIN_NAME_SPACE(tGis, Core)

SimpleMarkerSymbol::SimpleMarkerSymbol()
	:SimpleMarkerSymbol(SimpleMarkerSymbol::Rect)
{

}

SimpleMarkerSymbol::SimpleMarkerSymbol(int t)
{
	_type = t;
	_r = 30;
	_g = 236;
	_b = 63;
	_a = 255;
	_width = 5;
	_height = 5;
	_lineWidth = 1;
}

SimpleMarkerSymbol::~SimpleMarkerSymbol()
{

}

const ISymbolLibrary * SimpleMarkerSymbol::GetSymbolLibrary()
{
	return &SimpleSymbolLibrary::INSTANCE;;
}

const int SimpleMarkerSymbol::GetIdentifier()
{
	return IdentifierBegin + _type;
}

void SimpleMarkerSymbol::Paint(ISurface * surf, int count, int * x, int * y, int * z, char * c)
{
	switch (_type)
	{
	case Rect:
		DrawRect(surf, count, x, y, z);
		break;
	case Ellipse:
		DrawEllipse(surf, count, x, y, z);
		break;
	case Triangle:
		DrawTriangle(surf, count, x, y, z);
		break;
	case FillRect:
		DrawFillRect(surf, count, x, y, z);
		break;
	case FillEllipse:
		DrawFillEllipse(surf, count, x, y, z);
		break;
	case FillTriangle:
		DrawFillTriangle(surf, count, x, y, z);
		break;
	case Cross:
		DrawCross(surf, count, x, y, z);
		break;
	case EllipseCross:
		DrawEllipseCross(surf, count, x, y, z);
		break;
	default:
		DrawFillRect(surf, count, x, y, z);
		break;
	}
}

void SimpleMarkerSymbol::GetColor(unsigned char * r, unsigned char * g, unsigned char * b, unsigned char * a)
{
	*r = _r;
	*g = _g;
	*b = _b;
	*a = _a;
}

void SimpleMarkerSymbol::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}

int SimpleMarkerSymbol::GetWidth()
{
	return _width;
}

void SimpleMarkerSymbol::SetWidth(int w)
{
	_width = w;
}

int SimpleMarkerSymbol::GetHeight()
{
	return _height;
}

void SimpleMarkerSymbol::SetHeight(int h)
{
	_height = h;
}

int SimpleMarkerSymbol::GetXOffset()
{
	return _xOffset;
}

void SimpleMarkerSymbol::SetXOffset(int xOff)
{
	_xOffset = xOff;
}

int SimpleMarkerSymbol::GetYOffset()
{
	return _yOffset;
}

void SimpleMarkerSymbol::SetYOffset(int yOff)
{
	_yOffset = yOff;
}

void SimpleMarkerSymbol::DrawRect(ISurface * surf, int count, int * x, int * y, int * z)
{
	int prex = -_width - _xOffset;
	int prey = _height - _yOffset;
	for (int i = 0; i < count; i++)
	{
		bool draw = (prex != x[i] || prey != y[i]);
		prex = x[i];
		prey = y[i];
		if(draw)
			surf->DrawRect(x[i] + _xOffset, y[i] + _yOffset, _width, _height, _r, _g, _b, _a, _lineWidth, SimpleLineSymbol::Solid);
	}
}

void SimpleMarkerSymbol::DrawEllipse(ISurface * surf, int count, int * x, int * y, int * z)
{
	for (int i = 0; i < count; i++)
	{
		surf->DrawEllipse(x[i] + _xOffset, y[i] + _yOffset, _width, _height, _r, _g, _b, _a, _lineWidth, SimpleLineSymbol::Solid);
	}
}

void SimpleMarkerSymbol::DrawTriangle(ISurface * surf, int count, int * x, int * y, int * z)
{
	for (int i = 0; i < count; i++)
	{
		int hw = _width / 2;
		int tx[3] = { x[i] + _xOffset,x[i] + _xOffset + hw,x[i] + _xOffset + _width };
		int ty[3] = { y[i] + _yOffset + _height,y[i] + _yOffset,y[i] + _yOffset + _height };

		surf->DrawPolygon(3, tx, ty, _r, _g, _b, _a, _lineWidth, SimpleLineSymbol::Solid);
	}
}

void SimpleMarkerSymbol::DrawFillRect(ISurface * surf, int count, int * x, int * y, int * z)
{
	for (int i = 0; i < count; i++)
	{
		surf->FillRect(x[i] + _xOffset, y[i] + _yOffset, _width, _height, _r, _g, _b, _a, SimpleFillSymbol::Solid);
	}
}

void SimpleMarkerSymbol::DrawFillEllipse(ISurface * surf, int count, int * x, int * y, int * z)
{
	for (int i = 0; i < count; i++)
	{
		surf->FillEllipse(x[i] + _xOffset, y[i] + _yOffset, _width, _height, _r, _g, _b, _a, SimpleFillSymbol::Solid);
	}
}

void SimpleMarkerSymbol::DrawFillTriangle(ISurface * surf, int count, int * x, int * y, int * z)
{
	for (int i = 0; i < count; i++)
	{
		int hw = _width / 2;
		int tx[3] = { x[i] + _xOffset,x[i] + _xOffset + hw,x[i] + _xOffset + _width };
		int ty[3] = { y[i] + _yOffset + _height,y[i] + _yOffset,y[i] + _yOffset + _height };

		surf->FillPolygon(3, tx, ty, _r, _g, _b, _a, SimpleFillSymbol::Solid);
	}
}

void SimpleMarkerSymbol::DrawCross(ISurface * surf, int count, int * x, int * y, int * z)
{
	for (int i = 0; i < count; i++)
	{
		int hw = _width / 2;
		int hh = _height / 2;

		int l1x[2] = { x[i] + _xOffset,x[i] + _xOffset + _width };
		int l1y[2] = { y[i] + _yOffset + hh,y[i] + _yOffset + hh };
		int l2x[2] = { x[i] + _xOffset + hw,x[i] + _xOffset + hw };
		int l2y[2] = { y[i] + _yOffset,y[i] + _yOffset + _height };

		surf->DrawPolyline(2, l1x, l1y, _r, _g, _b, _a, _lineWidth, SimpleLineSymbol::Solid);
		surf->DrawPolyline(2, l2x, l2y, _r, _g, _b, _a, _lineWidth, SimpleLineSymbol::Solid);
	}
}

void SimpleMarkerSymbol::DrawEllipseCross(ISurface * surf, int count, int * x, int * y, int * z)
{
	for (int i = 0; i < count; i++)
	{
		int hw = _width / 2;
		int hh = _height / 2;

		int l1x[2] = { x[i] + _xOffset,x[i] + _xOffset + _width };
		int l1y[2] = { y[i] + _yOffset + hh,y[i] + _yOffset + hh };
		int l2x[2] = { x[i] + _xOffset + hw,x[i] + _xOffset + hw };
		int l2y[2] = { y[i] + _yOffset,y[i] + _yOffset + _height };

		surf->DrawPolyline(2, l1x, l1y, _r, _g, _b, _a, _lineWidth, SimpleLineSymbol::Solid);
		surf->DrawPolyline(2, l2x, l2y, _r, _g, _b, _a, _lineWidth, SimpleLineSymbol::Solid);
		surf->DrawEllipse(x[i] + _xOffset, y[i] + _yOffset, _width, _height, _r, _g, _b, _a, _lineWidth, SimpleLineSymbol::Solid);
	}
}

END_NAME_SPACE(tGis, Core)