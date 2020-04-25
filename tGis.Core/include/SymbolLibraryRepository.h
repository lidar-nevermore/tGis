#pragma once

#ifndef __SYMBOLLIBRARYREPOSITORY_H__
#define __SYMBOLLIBRARYREPOSITORY_H__

#include "Helper.h"
#include "tGis_CoreCfg.h"


BEGIN_NAME_SPACE(tGis, Core)

struct ISymbolLibrary;
class SymbolLibraryRepositoryImpl;

class TGIS_CORE_API SymbolLibraryRepository
{
public:
	static SymbolLibraryRepository* _instance;
	static SymbolLibraryRepository* INSTANCE();

public:
	~SymbolLibraryRepository();

private:
	SymbolLibraryRepository();
	SymbolLibraryRepository(const SymbolLibraryRepository &) = delete;
	SymbolLibraryRepository &operator=(const SymbolLibraryRepository &) = delete;

public:
	size_t GetMarkerSymbolLibraryCount();
	ISymbolLibrary* GetMarkerSymbolLibrary(size_t);

	size_t GetLineSymbolLibraryCount();
	ISymbolLibrary* GetLineSymbolLibrary(size_t);

	size_t GetFillSymbolLibraryCount();
	ISymbolLibrary* GetFillSymbolLibrary(size_t);

	void AddSymbolLibrary(ISymbolLibrary*);
	ISymbolLibrary* GetSymbolLibrary(const char* symLibName);

private:
	SymbolLibraryRepositoryImpl* _impl_;
};


END_NAME_SPACE(tGis, Core)


#endif

