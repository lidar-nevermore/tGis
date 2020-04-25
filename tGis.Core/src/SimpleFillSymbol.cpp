#include "SimpleFillSymbol.h"
#include "SimpleSymbolLibrary.h"


#include "ISurface.h"

#include "glad.h"
#include "glutess.h"


BEGIN_NAME_SPACE(tGis, Core)


SimpleFillSymbol::SimpleFillSymbol()
	:SimpleFillSymbol(SimpleFillSymbol::Solid)
{
	_lib = SimpleSymbolLibrary::GetFillSymbolLibrary();
}

SimpleFillSymbol::SimpleFillSymbol(int t)
{
	_lib = SimpleSymbolLibrary::GetFillSymbolLibrary();

	_type = t;
	_r = 30;
	_g = 236;
	_b = 63;
	_a = 255;
}


SimpleFillSymbol::~SimpleFillSymbol()
{
}

const int SimpleFillSymbol::GetId()
{
	return _type;
}


void SimpleFillSymbol::Paint(ISurface * surf, int count, int * x, int * y)
{
	//TODO: 调用OpenGL绘制填充面
	
}

void SimpleFillSymbol::Paint(ISurface * surf, int * count, int ** x, int ** y)
{

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

