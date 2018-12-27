#pragma once

#ifndef __SIMPLESYMBOLLIBRARY_H__
#define __SIMPLESYMBOLLIBRARY_H__

#include "Helper.h"
#include "ISymbolLibrary.h"

BEGIN_NAME_SPACE(tGis, Core)

struct ISymbol;

class TGIS_CORE_API SimpleSymbolLibrary : public ISymbolLibrary
{
public:
	static SimpleSymbolLibrary INSTANCE;

private:
	SimpleSymbolLibrary();
	~SimpleSymbolLibrary();
	SimpleSymbolLibrary(const SimpleSymbolLibrary &) = delete;
	SimpleSymbolLibrary &operator=(const SimpleSymbolLibrary &) = delete;

public:
	const char* GetType() const;
	const char* GetName() const;
	int GetSymbolCount() const;
	ISymbol* GetSymbol(int) const;
	ISymbol* GetSymbol(int, int*) const;
	void ReclaimSymbol(ISymbol*) const;

	void Release();

private:
	static const char* const _name;
	static const char* const _type;
};


END_NAME_SPACE(tGis, Core)


#endif

