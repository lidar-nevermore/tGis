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
public:
	static const int Solid = 0;
	static const int DenseDot1 = 1;
	static const int DenseDot2 = 2;
	static const int DenseDot3 = 3;
	static const int DenseDot4 = 4;
	static const int DenseDot5 = 5;
	static const int DenseDot6 = 6;
	static const int DenseDot7 = 7;
	static const int Horizontal = 8;
	static const int Vertical = 9;
	static const int ForwardDiagonal = 10;
	static const int BackwardDiagonal = 11;
	static const int Cross = 12;
	static const int DiagonalCross = 13;
	static const int MaxId = 13;

public:
	SimpleFillSymbol();
	SimpleFillSymbol(int t);
	virtual ~SimpleFillSymbol();

	const int GetId();

	void Paint(ISurface* surf, int count, int* x, int* y) override;
	void Paint(ISurface* surf, int contourCount, int* ptCount, int** x, int** y) override;

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


