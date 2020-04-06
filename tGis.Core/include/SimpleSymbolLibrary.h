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
	const char* GetName() const;
	int GetSymbolCount() const;
	ISymbol* GetSymbol(int id) const;
	ISymbol* GetSymbol(int id, int* nextId) const;
	void RevertSymbol(ISymbol*) const;

private:
	static const char* const _name;
};


END_NAME_SPACE(tGis, Core)


#endif

