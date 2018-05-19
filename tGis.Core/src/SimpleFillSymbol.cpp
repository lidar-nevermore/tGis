#include "SimpleFillSymbol.h"


#include "ISurface.h"

#include <cstdlib>


BEGIN_NAME_SPACE(tGis, Core)

SimpleFillSymbol::SimpleFillSymbol(int t)
{
	_type = t;
	_r = 30;
	_g = 236;
	_b = 63;
	_a = 0;

	_locator[0] = 's';
	_locator[1] = 'y';
	_locator[2] = 's';
	_locator[3] = '_';
	_locator[4] = 'f';
	_locator[5] = 'i';
	_locator[6] = 'l';
	_locator[7] = 'l';
	_itoa(t, _locator + 8, 10);
}


SimpleFillSymbol::~SimpleFillSymbol()
{
}

const char * SimpleFillSymbol::GetLocator()
{
	return _locator;
}

void SimpleFillSymbol::Paint(ISurface * surf, int count, int * x, int * y, int * z, char * c)
{
	surf->FillPolygon(count, x, y, _r, _g, _b, _a, _type);
}

inline void SimpleFillSymbol::GetColor(unsigned char * r, unsigned char * g, unsigned char * b, unsigned char * a)
{
	*r = _r;
	*g = _g;
	*b = _b;
	*a = _a;
}

inline void SimpleFillSymbol::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}


END_NAME_SPACE(tGis, Core)

