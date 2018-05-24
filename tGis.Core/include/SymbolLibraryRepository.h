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

class SymbolLibraryRepository
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
	int GetSymbolLibraryCount();
	ISymbolLibrary* GetSymbolLibrary(int);
	void AddSymbolLibrary(ISymbolLibrary*);
	ISymbolLibrary* GetSymbolLibrary(const char* symbolLibraryType);

private:
	vector<ISymbolLibrary*> _vecSymbolLibrary;
	map<string, ISymbolLibrary*> _mapSymbolLibrary;
};


END_NAME_SPACE(tGis, Core)


#endif

