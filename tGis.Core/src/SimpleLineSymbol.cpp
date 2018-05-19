#include "SimpleLineSymbol.h"

#include "ISurface.h"

#include <cstdlib>

BEGIN_NAME_SPACE(tGis, Core)

SimpleLineSymbol::SimpleLineSymbol(int t)
{
	_type = t;
	_r = 30;
	_g = 236;
	_b = 63;
	_a = 0;
	_width = 1;

	_locator[0] = 's';
	_locator[1] = 'y';
	_locator[2] = 's';
	_locator[3] = '_';
	_locator[4] = 'l';
	_locator[5] = 'i';
	_locator[6] = 'n';
	_locator[7] = 'e';
	_itoa(t, _locator + 8, 10);
}

SimpleLineSymbol::~SimpleLineSymbol()
{
}

const char * SimpleLineSymbol::GetLocator()
{
	return _locator;
}

void SimpleLineSymbol::Paint(ISurface * surf, int count, int * x, int * y, int * z, char * c)
{
	surf->DrawPolyline(count, x, y, _r, _g, _b, _a, _width, _type);
}

inline void SimpleLineSymbol::GetColor(unsigned char * r, unsigned char * g, unsigned char * b, unsigned char * a)
{
	*r = _r;
	*g = _g;
	*b = _b;
	*a = _a;
}

inline void SimpleLineSymbol::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	_r = r;
	_g = g;
	_b = b;
	_a = a;
}

inline int SimpleLineSymbol::GetWidth()
{
	return _width;
}

inline void SimpleLineSymbol::SetWidth(int w)
{
	_width = w;
}


END_NAME_SPACE(tGis, Core)

