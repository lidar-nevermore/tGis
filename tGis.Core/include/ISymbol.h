#pragma once

#ifndef __I_SYMBOL_H__
#define __I_SYMBOL_H__

#include "Helper.h"

BEGIN_NAME_SPACE(tGis, Core)

struct ISurface;

struct TGISCORE_API ISymbol
{
	virtual const char* GetLocator() = 0;

	virtual void Paint(ISurface* surf, int count, int* x, int* y, int* z, void* c) = 0;

	virtual ~ISymbol() {};
};

template<class T>
struct TSymbol : public ISymbol
{
	virtual void Paint(ISurface* surf, int count, int* x, int* y, int* z, T* c) = 0;

	virtual ~TSymbol() {};

private:
	void Paint(ISurface* surf, int count, int* x, int* y, int* z, void* c)
	{
		this->Paint(surf, count, x, y, z, (T*)c);
	}
};

END_NAME_SPACE(tGis, Core)


#endif
