#pragma once

#ifndef __SIMPLEFILLSYMBOL_H__
#define __SIMPLEFILLSYMBOL_H__

#include "Helper.h"
#include "IFillSymbol.h"

class GLUtesselator;

BEGIN_NAME_SPACE(tGis, Core)

struct ISurface;

class TGIS_CORE_API SimpleFillSymbol : public IFillSymbol
{
	friend class SimpleSymbolLibrary;

public:
	static const int NoFill = 0;
	static const int Solid = 1;
	static const int DenseDot1 = 2;
	static const int DenseDot2 = 3;
	static const int DenseDot3 = 4;
	static const int DenseDot4 = 5;
	static const int DenseDot5 = 6;
	static const int DenseDot6 = 7;
	static const int DenseDot7 = 8;
	static const int Horizontal = 9;
	static const int Vertical = 10;
	static const int ForwardDiagonal = 11;
	static const int BackwardDiagonal = 12;
	static const int Cross = 13;
	static const int DiagonalCross = 14;
	static const int MaxId = 14;

public:
	SimpleFillSymbol();
	SimpleFillSymbol(unsigned char r, unsigned char g, unsigned char b, unsigned char a, int t);
	virtual ~SimpleFillSymbol();

protected:
	SimpleFillSymbol(int t, const ISymbolLibrary* symLib);

public:
	int GetId();

	void Paint(ISurface* surf, int count, int* x, int* y) override;
	void Paint(ISurface* surf, int contourCount, int* ptCount, int** x, int** y) override;

	void BeginPaint(ISurface* surf) override;

	void BeginContour(ISurface* surf) override;

	void AppendVertex(int count, int* x, int* y) override;

	void AppendVertex(int x, int y) override;

	void EndContour(ISurface* surf) override;

	void EndPaint(ISurface* surf) override;

public:
	void GetColor(unsigned char* r, unsigned char* g, unsigned char* b, unsigned char* a);
	void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

protected:
	unsigned char _r;
	unsigned char _g;
	unsigned char _b;
	unsigned char _a;

	int _type;
};


END_NAME_SPACE(tGis, Core)


#endif


