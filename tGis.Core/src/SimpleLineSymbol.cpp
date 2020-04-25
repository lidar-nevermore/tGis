#include "SimpleLineSymbol.h"
#include "SimpleSymbolLibrary.h"

#include "ISurface.h"
#include "glad.h"


BEGIN_NAME_SPACE(tGis, Core)

int g_pattern[] =   
{
	//实线
	0xFFFF,

	//虚线  1111110011111100   dashed
	//____  ____  ____  ____  ____  ____  ____
	0x3F3F,

	//点线 1000100010001000, 表示实际画线的点，反序后转换成16进制就是0x1111 dotted
	//.  .  .  .  .  .  .  .  .  .  .  .  .  .
	0x1111,

	//点划线    1111111111100100  dot dash
	//____ . ____ . _____ . _____. _____
	0x27FF,

	//双点划线  1111111100100100  double dot dash
	// ____ . . ____ . . ____ . . ____ . . ____
	0x24FF,

	//三点划线  111111110101010 tri_dot_dash
	// ____ . . . ____ . . . ____ . . . ____ 
	0x55FF,

	//中心线    1111111111001100  centre line
	//_____ _ _____ _ _____ _ _____ _ _____
	0x33FF,
};

SimpleLineSymbol::SimpleLineSymbol()
	:SimpleLineSymbol(SimpleLineSymbol::Solid)
{
	_lib = SimpleSymbolLibrary::GetLineSymbolLibrary();

}

SimpleLineSymbol::SimpleLineSymbol(int t)
{
	_lib = SimpleSymbolLibrary::GetLineSymbolLibrary();

	_type = t;
	if (_type < 0 || _type > 5)
		_type = 0;
	_r = 0;
	_g = 0;
	_b = 0;
	_a = 255;
	_width = 1;
}

SimpleLineSymbol::~SimpleLineSymbol()
{
}

const int SimpleLineSymbol::GetId()
{
	return _type;
}

void SimpleLineSymbol::Paint(ISurface * surf, int count, int * x, int * y)
{
	int surfWidth;
	int surfHeight;
	surf->GetSize(&surfWidth, &surfHeight);
	GLfloat red = _r / 255.0f;
	GLfloat green = _g / 255.0f;
	GLfloat blue = _g / 255.0f;
	GLfloat alpha = _a / 255.0f;

	glLineWidth((GLfloat)_width);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1, g_pattern[_type]);
	glColor4f(red, green, blue, alpha);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < count; i++)
	{
		GLfloat xg = (2.0f*x[i]) / surfWidth - 1.0f;
		GLfloat yg = 1.0f - (2.0f*y[i]) / surfHeight;
		glVertex3f(xg, yg, 0.0f);
	}	
	glEnd();
	glDisable(GL_LINE_STIPPLE);
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

