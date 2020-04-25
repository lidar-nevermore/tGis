#pragma once

#ifndef __I_SYMBOL_H__
#define __I_SYMBOL_H__

#include "Helper.h"
#include "ITypedObject.h"

BEGIN_NAME_SPACE(tGis, Core)

struct ISurface;
struct ISymbolLibrary;

struct TGIS_CORE_API ISymbol : public ITypedObject
{
	virtual const ISymbolLibrary* GetSymbolLibrary()
	{
		return _lib;
	};

	virtual const int GetId() = 0;

	virtual ~ISymbol() {};

protected:
	ISymbolLibrary* _lib;
};


typedef ISymbol* ISymbolPtr;

END_NAME_SPACE(tGis, Core)


#endif
