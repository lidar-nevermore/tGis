#include "SimpleMarkerSymbol.h"
#include "SimpleSymbolLibrary.h"

#include "ISurface.h"

#include "glad.h"
#include "glutess.h"

BEGIN_NAME_SPACE(tGis, Core)

SimpleMarkerSymbol::SimpleMarkerSymbol()
	:SimpleMarkerSymbol(SimpleMarkerSymbol::Rect)
{
	_lib = SimpleSymbolLibrary::GetMarkerSymbolLibrary();

}

SimpleMarkerSymbol::SimpleMarkerSymbol(int t)
{
	_lib = SimpleSymbolLibrary::GetMarkerSymbolLibrary();

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

const int SimpleMarkerSymbol::GetId()
{
	return _type;
}

void SimpleMarkerSymbol::Paint(ISurface * surf, int count, int * x, int * y)
{
	GLfloat red = _r / 255.0f;
	GLfloat green = _g / 255.0f;
	GLfloat blue = _g / 255.0f;
	GLfloat alpha = _a / 255.0f;

	glLineWidth((GLfloat)_lineWidth);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glColor4f(red, green, blue, alpha);

	switch (_type)
	{
	case Rect:
		DrawRect(surf, count, x, y);
		break;
	case Ellipse:
		DrawEllipse(surf, count, x, y);
		break;
	case Triangle:
		DrawTriangle(surf, count, x, y);
		break;
	case FillRect:
		DrawFillRect(surf, count, x, y);
		break;
	case FillEllipse:
		DrawFillEllipse(surf, count, x, y);
		break;
	case FillTriangle:
		DrawFillTriangle(surf, count, x, y);
		break;
	case Cross:
		DrawCross(surf, count, x, y);
		break;
	case EllipseCross:
		DrawEllipseCross(surf, count, x, y);
		break;
	default:
		DrawFillRect(surf, count, x, y);
		break;
	}
}

void SimpleMarkerSymbol::Paint(ISurface* surf, int x, int y)
{
	GLfloat red = _r / 255.0f;
	GLfloat green = _g / 255.0f;
	GLfloat blue = _g / 255.0f;
	GLfloat alpha = _a / 255.0f;

	glLineWidth((GLfloat)_lineWidth);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glColor4f(red, green, blue, alpha);

	switch (_type)
	{
	case Rect:
		DrawRect(surf, 1, &x, &y);
		break;
	case Ellipse:
		DrawEllipse(surf, 1, &x, &y);
		break;
	case Triangle:
		DrawTriangle(surf, 1, &x, &y);
		break;
	case FillRect:
		DrawFillRect(surf, 1, &x, &y);
		break;
	case FillEllipse:
		DrawFillEllipse(surf, 1, &x, &y);
		break;
	case FillTriangle:
		DrawFillTriangle(surf, 1, &x, &y);
		break;
	case Cross:
		DrawCross(surf, 1, &x, &y);
		break;
	case EllipseCross:
		DrawEllipseCross(surf, 1, &x, &y);
		break;
	default:
		DrawFillRect(surf, 1, &x, &y);
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

int SimpleMarkerSymbol::GetLineWidth()
{
	return _lineWidth;
}

void SimpleMarkerSymbol::SetLineWidth(int lw)
{
	_lineWidth = lw;
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

void SimpleMarkerSymbol::DrawRect(ISurface * surf, int count, int * x, int * y)
{
	int prex = -1;
	int prey = -1;
	for (int i = 0; i < count; i++)
	{
		bool draw = (prex != x[i] || prey != y[i]);
		prex = x[i];
		prey = y[i];
		if (draw)
		{
			int sleft = x[i] + _xOffset;
			int stop = y[i] + _yOffset;
			int sright = sleft + _width;
			int sbottom = stop + _height;
			GLfloat left, top, right, bottom;
			surf->Surface2glndc(sleft, stop, &left, &top);
			surf->Surface2glndc(sright, sbottom, &right, &bottom);

			glBegin(GL_LINE_STRIP);
			glVertex3f(left, top, 0.0f);
			glVertex3f(right, top, 0.0f);
			glVertex3f(right, bottom, 0.0f);
			glVertex3f(left, bottom, 0.0f);
			glVertex3f(left, top, 0.0f);
			glEnd();
		}
	}
}

void SimpleMarkerSymbol::DrawEllipse(ISurface * surf, int count, int * x, int * y)
{
	for (int i = 0; i < count; i++)
	{
		//TODO: 调用OpenGL绘制
		;
	}
}

void SimpleMarkerSymbol::DrawTriangle(ISurface * surf, int count, int * x, int * y)
{
	for (int i = 0; i < count; i++)
	{
		int hw = _width / 2;
		int tx[3] = { x[i] + _xOffset,x[i] + _xOffset + hw,x[i] + _xOffset + _width };
		int ty[3] = { y[i] + _yOffset + _height,y[i] + _yOffset,y[i] + _yOffset + _height };

		{
			//TODO: 调用OpenGL绘制
			;
		}
	}
}

void SimpleMarkerSymbol::DrawFillRect(ISurface * surf, int count, int * x, int * y)
{
	for (int i = 0; i < count; i++)
	{
		//TODO: 调用OpenGL绘制
		;
	}
}

void SimpleMarkerSymbol::DrawFillEllipse(ISurface * surf, int count, int * x, int * y)
{
	for (int i = 0; i < count; i++)
	{
		//TODO: 调用OpenGL绘制
		;
	}
}

void SimpleMarkerSymbol::DrawFillTriangle(ISurface * surf, int count, int * x, int * y)
{
	for (int i = 0; i < count; i++)
	{
		int hw = _width / 2;
		int tx[3] = { x[i] + _xOffset,x[i] + _xOffset + hw,x[i] + _xOffset + _width };
		int ty[3] = { y[i] + _yOffset + _height,y[i] + _yOffset,y[i] + _yOffset + _height };

		{
			//TODO: 调用OpenGL绘制
			;
		}
	}
}

void SimpleMarkerSymbol::DrawCross(ISurface * surf, int count, int * x, int * y)
{
	for (int i = 0; i < count; i++)
	{
		int hw = _width / 2;
		int hh = _height / 2;

		int l1x[2] = { x[i] + _xOffset,x[i] + _xOffset + _width };
		int l1y[2] = { y[i] + _yOffset + hh,y[i] + _yOffset + hh };
		int l2x[2] = { x[i] + _xOffset + hw,x[i] + _xOffset + hw };
		int l2y[2] = { y[i] + _yOffset,y[i] + _yOffset + _height };

		{
			//TODO: 调用OpenGL绘制
			;
		}
	}
}

void SimpleMarkerSymbol::DrawEllipseCross(ISurface * surf, int count, int * x, int * y)
{
	for (int i = 0; i < count; i++)
	{
		int hw = _width / 2;
		int hh = _height / 2;

		int l1x[2] = { x[i] + _xOffset,x[i] + _xOffset + _width };
		int l1y[2] = { y[i] + _yOffset + hh,y[i] + _yOffset + hh };
		int l2x[2] = { x[i] + _xOffset + hw,x[i] + _xOffset + hw };
		int l2y[2] = { y[i] + _yOffset,y[i] + _yOffset + _height };

		{
			//TODO: 调用OpenGL绘制
			;
		}
	}
}

END_NAME_SPACE(tGis, Core)