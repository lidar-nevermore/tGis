#pragma once

#ifndef __SIMPLEFILLSYMBOL_H__
#define __SIMPLEFILLSYMBOL_H__

#include "Helper.h"
#include "ISymbol.h"

BEGIN_NAME_SPACE(tGis, Core)

struct ISurface;

class TGISCORE_API SimpleFillSymbol : public TSymbol<char>
{
public:
	static const int NoFill = -1;
	static const int Solid = 0;
	static const int Dense1 = 1;
	static const int Dense2 = 2;
	static const int Dense3 = 3;
	static const int Dense4 = 4;
	static const int Dense5 = 5;
	static const int Dense6 = 6;
	static const int Dense7 = 7;
	static const int Horizontal = 8;
	static const int Vertical = 9;
	static const int ForwardDiagonal = 10;
	static const int BackwardDiagonal = 11;
	static const int Cross = 12;
	static const int DiagonalCross = 13;

public:
	SimpleFillSymbol(int t);
	~SimpleFillSymbol();

	virtual const char* GetLocator() = 0;

	virtual void Paint(ISurface* surf, int count, int* x, int* y, int* z, char* c) = 0;

public:
	inline void GetColor(unsigned char* r, unsigned char* g, unsigned char* b, unsigned char* a);
	inline void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

protected:
	unsigned char _r;
	unsigned char _g;
	unsigned char _b;
	unsigned char _a;

	char _locator[32];
	int _type;
};


END_NAME_SPACE(tGis, Core)


#endif


