#include "SimpleLineSymbol.h"
#include "SimpleSymbolLibrary.h"

#include "ISurface.h"


BEGIN_NAME_SPACE(tGis, Core)

SimpleLineSymbol::SimpleLineSymbol()
	:SimpleLineSymbol(SimpleLineSymbol::Solid)
{
}

SimpleLineSymbol::SimpleLineSymbol(int t)
{
	_type = t;
	_r = 0;
	_g = 0;
	_b = 0;
	_a = 255;
	_width = 1;
}

SimpleLineSymbol::~SimpleLineSymbol()
{
}

const ISymbolLibrary * SimpleLineSymbol::GetSymbolLibrary()
{
	return &SimpleSymbolLibrary::INSTANCE;;
}

const int SimpleLineSymbol::GetIdentifier()
{
	return IdentifierBegin + _type;
}

void SimpleLineSymbol::Paint(ISurface * surf, int count, int * x, int * y, int * z, char * c)
{
	surf->DrawPolyline(count, x, y, _r, _g, _b, _a, _width, _type);
}

void SimpleLineSymbol::GetColor(unsigned char * r, unsigned char * g, unsigned char * b, unsigned char * a)
{
	*r = _r;
	*g = _g;
	*b = _b;
	*a = _a;
}

void SimpleLineSymbol::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}

int SimpleLineSymbol::GetWidth()
{
	return _width;
}

void SimpleLineSymbol::SetWidth(int w)
{
	_width = w;
}


END_NAME_SPACE(tGis, Core)

