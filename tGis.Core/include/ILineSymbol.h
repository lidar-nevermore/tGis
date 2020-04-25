#pragma once

#ifndef __I_LineSymbol_H__
#define __I_LineSymbol_H__

#include "Helper.h"
#include "ISymbol.h"

BEGIN_NAME_SPACE(tGis, Core)


struct TGIS_CORE_API ILineSymbol : public ISymbol
{
public:
	virtual const char* GetType();
	static const char* S_GetType();
	virtual bool IsTypeOf(const char* type);
	//一次画一条线
	virtual void Paint(ISurface* surf, int count, int* x, int* y) = 0;

private:
	static const char* const _type;
};


END_NAME_SPACE(tGis, Core)


#endif
