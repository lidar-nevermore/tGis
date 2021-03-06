#pragma once

#ifndef __SIMPLEMARKERSYMBOL_H__
#define __SIMPLEMARKERSYMBOL_H__

#include "Helper.h"
#include "IMarkerSymbol.h"

BEGIN_NAME_SPACE(tGis, Core)

struct ISurface;

class TGIS_CORE_API SimpleMarkerSymbol : public IMarkerSymbol
{
	friend class SimpleSymbolLibrary;
public:
	static const int Rect = 0;
	static const int Ellipse = 1;
	static const int Triangle = 2;
	static const int FillRect = 3;
	static const int FillEllipse = 4;
	static const int FillTriangle = 5;
	static const int Cross = 6;
	static const int EllipseCross = 7;
	static const int MaxId = 7;

public:
	SimpleMarkerSymbol();
	SimpleMarkerSymbol(unsigned char r, unsigned char g, unsigned char b, unsigned char a, int t);
	virtual ~SimpleMarkerSymbol();

protected:
	SimpleMarkerSymbol(int t, const ISymbolLibrary* symLib);

public:
	int GetId();

	void Paint(ISurface* surf, int count, int* x, int* y) override;
	void Paint(ISurface* surf, int x, int y) override;

    void GetColor(unsigned char* r, unsigned char* g, unsigned char* b, unsigned char* a);
	void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	int GetWidth();
	void SetWidth(int w);
	int GetHeight();
	void SetHeight(int h);
	int GetLineWidth();
	void SetLineWidth(int lw);
	int GetXOffset();
	void SetXOffset(int xOff);
	int GetYOffset();
	void SetYOffset(int yOff);

protected:
	void DrawRect(ISurface* surf, int count, int* x, int* y);
	void DrawEllipse(ISurface* surf, int count, int* x, int* y);
	void DrawTriangle(ISurface* surf, int count, int* x, int* y);
	void DrawFillRect(ISurface* surf, int count, int* x, int* y);
	void DrawFillEllipse(ISurface* surf, int count, int* x, int* y);
	void DrawFillTriangle(ISurface* surf, int count, int* x, int* y);
	void DrawCross(ISurface* surf, int count, int* x, int* y);
	void DrawEllipseCross(ISurface* surf, int count, int* x, int* y);

protected:
	unsigned char _r;
	unsigned char _g;
	unsigned char _b;
	unsigned char _a;

	int _width;
	int _halfWidth;
	int _height;
	int _halfHeight;
	int _xOffset;
	int _yOffset;

    int _lineWidth;

	int _type;
};


END_NAME_SPACE(tGis, Core)


#endif




