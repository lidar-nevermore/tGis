#include "SimpleFillSymbol.h"
#include "SimpleSymbolLibrary.h"


#include "ISurface.h"


BEGIN_NAME_SPACE(tGis, Core)


SimpleFillSymbol::SimpleFillSymbol()
	:SimpleFillSymbol(SimpleFillSymbol::Solid)
{
}

SimpleFillSymbol::SimpleFillSymbol(int t)
{
	_type = t;
	_r = 30;
	_g = 236;
	_b = 63;
	_a = 255;
}


SimpleFillSymbol::~SimpleFillSymbol()
{
}

const ISymbolLibrary * SimpleFillSymbol::GetSymbolLibrary()
{
	return &SimpleSymbolLibrary::INSTANCE;
}

const int SimpleFillSymbol::GetIdentifier()
{
	return IdentifierBegin + _type;
}

void SimpleFillSymbol::Paint(ISurface * surf, int count, int * x, int * y, int * z, char * c)
{
	surf->FillPolygon(count, x, y, _r, _g, _b, _a, _type);
}

void SimpleFillSymbol::GetColor(unsigned char * r, unsigned char * g, unsigned char * b, unsigned char * a)
{
	*r = _r;
	*g = _g;
	*b = _b;
	*a = _a;
}

void SimpleFillSymbol::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}


END_NAME_SPACE(tGis, Core)

