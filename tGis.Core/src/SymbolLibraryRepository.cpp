#include "SymbolLibraryRepository.h"
#include "SimpleSymbolLibrary.h"

BEGIN_NAME_SPACE(tGis, Core)

SymbolLibraryRepository* SymbolLibraryRepository::_instance = nullptr;


SymbolLibraryRepository & SymbolLibraryRepository::INSTANCE()
{
	if (_instance == nullptr)
	{
		_instance = new SymbolLibraryRepository();
		static PtrDestructor<SymbolLibraryRepository> shit(_instance);
	}

	return *_instance;
}

SymbolLibraryRepository::SymbolLibraryRepository()
	:_mapSymbolLibrary()
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

size_t SymbolLibraryRepository::GetSymbolLibraryCount()
{
	return _vecSymbolLibrary.size();
}

ISymbolLibrary * SymbolLibraryRepository::GetSymbolLibrary(size_t pos)
{
	return _vecSymbolLibrary.at(pos);
}

void SymbolLibraryRepository::AddSymbolLibrary(ISymbolLibrary * sl)
{
	string type = sl->GetType();
	_vecSymbolLibrary.push_back(sl);
	_mapSymbolLibrary.insert(map<string, ISymbolLibrary*>::value_type(type, sl));
}

ISymbolLibrary * SymbolLibraryRepository::GetSymbolLibrary(const char * symbolLibraryType)
{
	map<string, ISymbolLibrary*>::iterator pos = _mapSymbolLibrary.find(symbolLibraryType);

	if (pos != _mapSymbolLibrary.end())
		return (*pos).second;
	return nullptr;
}


END_NAME_SPACE(tGis, Core)


