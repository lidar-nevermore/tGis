#pragma once

#ifndef __SYMBOLLIBRARYREPOSITORY_H__
#define __SYMBOLLIBRARYREPOSITORY_H__

#include "Helper.h"

#include <vector>
#include <map>
#include <string>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

struct ISymbolLibrary;

class TGIS_API SymbolLibraryRepository
{
public:
	static SymbolLibraryRepository* _instance;
	static SymbolLibraryRepository& INSTANCE();

public:
	~SymbolLibraryRepository();

private:
	SymbolLibraryRepository();
	SymbolLibraryRepository(const SymbolLibraryRepository &) = delete;
	SymbolLibraryRepository &operator=(const SymbolLibraryRepository &) = delete;

public:
	size_t GetSymbolLibraryCount();
	ISymbolLibrary* GetSymbolLibrary(size_t);
	void AddSymbolLibrary(ISymbolLibrary*);
	ISymbolLibrary* GetSymbolLibrary(const char* symbolLibraryType);

private:
	vector<ISymbolLibrary*> _vecSymbolLibrary;
	map<string, ISymbolLibrary*> _mapSymbolLibrary;
};


END_NAME_SPACE(tGis, Core)


#endif

