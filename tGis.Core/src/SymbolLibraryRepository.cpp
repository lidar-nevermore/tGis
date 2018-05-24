#include "SymbolLibraryRepository.h"
#include "ISymbolLibrary.h"
#include "SimpleSymbolLibrary.h"

BEGIN_NAME_SPACE(tGis, Core)

SymbolLibraryRepository SymbolLibraryRepository::INSTANCE;

SymbolLibraryRepository::SymbolLibraryRepository()
{
}


SymbolLibraryRepository::~SymbolLibraryRepository()
{
	for (vector<ISymbolLibrary*>::iterator it = _vecSymbolLibrary.begin(); it != _vecSymbolLibrary.end(); it++)
	{
		ISymbolLibrary* sl = *it;

		if (sl != &SimpleSymbolLibrary::INSTANCE)
			sl->Release();
	}
}

int SymbolLibraryRepository::GetSymbolLibraryCount()
{
	return _vecSymbolLibrary.size();
}

ISymbolLibrary * SymbolLibraryRepository::GetSymbolLibrary(int pos)
{
	return _vecSymbolLibrary.at(pos);
}

void SymbolLibraryRepository::AddSymbolLibrary(ISymbolLibrary * sl)
{
	string type = sl->GetType();
	_vecSymbolLibrary.push_back(sl);
	_mapSymbolLibrary.insert(make_pair(type, sl));
}

ISymbolLibrary * SymbolLibraryRepository::GetSymbolLibrary(const char * symbolLibraryType)
{
	map<string, ISymbolLibrary*>::iterator pos = _mapSymbolLibrary.find(symbolLibraryType);

	if (pos != _mapSymbolLibrary.end())
		return (*pos).second;
	return nullptr;
}


END_NAME_SPACE(tGis, Core)


