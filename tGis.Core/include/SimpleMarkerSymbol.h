#pragma once

#ifndef __SIMPLEMARKERSYMBOL_H__
#define __SIMPLEMARKERSYMBOL_H__

#include "Helper.h"
#include "ISymbol.h"

BEGIN_NAME_SPACE(tGis, Core)

struct ISurface;

class TGISCORE_API SimpleMarkerSymbol : public TSymbol<char>
{
public:
	static const int Rect = 0;
	static const int Ellipse = 1;
	static const int Triangle = 2;
	static const int FillRect = 3;
	static const int FillEllipse = 4;
	static const int FillTriangle = 5;
	static const int Cross = 6;
	static const int EllipseCross = 7;

public:
	SimpleMarkerSymbol(int t);
	~SimpleMarkerSymbol();

	virtual const char* GetLocator() = 0;

	virtual void Paint(ISurface* surf, int count, int* x, int* y, int* z, char* c);

    inline void GetColor(unsigned char* r, unsigned char* g, unsigned char* b, unsigned char* a);
	inline void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	inline int GetWidth();
	inline void SetWidth(int w);
	inline int GetHeight();
	inline void SetHeight(int h);
	inline int GetXOffset();
	inline void SetXOffset(int xOff);
	inline int GetYOffset();
	inline void SetYOffset(int yOff);

protected:
	void DrawRect(ISurface* surf, int count, int* x, int* y, int* z);
	void DrawEllipse(ISurface* surf, int count, int* x, int* y, int* z);
	void DrawTriangle(ISurface* surf, int count, int* x, int* y, int* z);
	void DrawFillRect(ISurface* surf, int count, int* x, int* y, int* z);
	void DrawFillEllipse(ISurface* surf, int count, int* x, int* y, int* z);
	void DrawFillTriangle(ISurface* surf, int count, int* x, int* y, int* z);
	void DrawCross(ISurface* surf, int count, int* x, int* y, int* z);
	void DrawEllipseCross(ISurface* surf, int count, int* x, int* y, int* z);

protected:
	unsigned char _r;
	unsigned char _g;
	unsigned char _b;
	unsigned char _a;

	int _width;
	int _height;
	int _xOffset;
	int _yOffset;

    int _lineWidth;

	char _locator[32];
	int _type;
};


END_NAME_SPACE(tGis, Core)


#endif




