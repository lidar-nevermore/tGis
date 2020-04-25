#pragma once

#ifndef __SIMPLELINESYMBOL_H__
#define __SIMPLELINESYMBOL_H__

#include "Helper.h"
#include "ILineSymbol.h"

BEGIN_NAME_SPACE(tGis, Core)

struct ISurface;

class TGIS_CORE_API SimpleLineSymbol : public ILineSymbol
{
public:
	static const int Solid = 0;
	static const int Dash = 1;
	static const int Dot = 2;
	static const int DashDot = 3;
	static const int DashDotDot = 4;
	static const int DashDotDotDot = 5;
	static const int MaxId = 5;

public:
	SimpleLineSymbol();
	SimpleLineSymbol(int t);
	~SimpleLineSymbol();

	const int GetId();

	void Paint(ISurface* surf, int count, int* x, int* y) override;

public:
	void GetColor(unsigned char* r, unsigned char* g, unsigned char* b, unsigned char* a);
	void SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	int GetWidth();
	void SetWidth(int w);

protected:
	unsigned char _r;
	unsigned char _g;
	unsigned char _b;
	unsigned char _a;

	int _width;

	int _type;
};


END_NAME_SPACE(tGis, Core)


#endif




