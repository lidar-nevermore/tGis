#pragma once

#ifndef __SIMPLEFILLSYMBOL_H__
#define __SIMPLEFILLSYMBOL_H__

#include "Helper.h"
#include "ISymbol.h"

BEGIN_NAME_SPACE(tGis, Core)

struct ISurface;

class TGIS_CORE_API SimpleFillSymbol : public TSymbol<char>
{
public:
	static const int IdentifierBegin = 200;
	static const int IdentifierEnd = 214;
	static const int NoFill = 0;
	static const int Solid = 1;
	static const int Dense1 = 2;
	static const int Dense2 = 3;
	static const int Dense3 = 4;
	static const int Dense4 = 5;
	static const int Dense5 = 6;
	static const int Dense6 = 7;
	static const int Dense7 = 8;
	static const int Horizontal = 9;
	static const int Vertical = 10;
	static const int ForwardDiagonal = 11;
	static const int BackwardDiagonal = 12;
	static const int Cross = 13;
	static const int DiagonalCross = 14;

public:
	SimpleFillSymbol();
	SimpleFillSymbol(int t);
	~SimpleFillSymbol();

	const ISymbolLibrary* GetSymbolLibrary();

	const int GetIdentifier();

	void Paint(ISurface* surf, int count, int* x, int* y, int* z, char* c) override;

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


