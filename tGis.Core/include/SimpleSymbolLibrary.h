#pragma once

#ifndef __SIMPLESYMBOLLIBRARY_H__
#define __SIMPLESYMBOLLIBRARY_H__

#include "Helper.h"
#include "ISymbolLibrary.h"

BEGIN_NAME_SPACE(tGis, Core)

struct ISymbol;

class TGIS_CORE_API SimpleSymbolLibrary : public ISymbolLibrary
{
	friend class SymbolLibraryRepository;

public:
	static SimpleSymbolLibrary* GetMarkerSymbolLibrary();
	static SimpleSymbolLibrary* GetLineSymbolLibrary();
	static SimpleSymbolLibrary* GetFillSymbolLibrary();

private:
	static SimpleSymbolLibrary* _markerSymbolLibrary;
	static SimpleSymbolLibrary* _lineSymbolLibrary;
	static SimpleSymbolLibrary* _fillSymbolLibrary;

private:
	SimpleSymbolLibrary(SymbolLibraryType libType);
	SimpleSymbolLibrary(const SimpleSymbolLibrary &) = delete;
	SimpleSymbolLibrary &operator=(const SimpleSymbolLibrary &) = delete;

public:
	~SimpleSymbolLibrary();

public:
	const char* GetName() const;
	int GetSymbolCount() const;
	bool SymbolExists(int id) const;
	ISymbol* GetSymbol(int id) const;
	bool SymbolExists(int id, int* nextId) const;
	ISymbol* GetSymbol(int id, int* nextId) const;

private:
	char* _name;
};


END_NAME_SPACE(tGis, Core)


#endif

