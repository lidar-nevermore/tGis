#pragma once

#ifndef __I_SYMBOL_H__
#define __I_SYMBOL_H__

#include "Helper.h"
#include "IInclusionObject.h"

BEGIN_NAME_SPACE(tGis, Core)

struct ISurface;
struct ISymbolLibrary;

struct TGIS_CORE_API ISymbol
{
	virtual const ISymbolLibrary* GetSymbolLibrary() = 0;

	virtual const int GetId() = 0;

	//count表示x,y,z数组的尺寸
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

typedef ISymbol* ISymbolPtr;

END_NAME_SPACE(tGis, Core)


#endif
