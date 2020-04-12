#include "SimpleSymbolLibrary.h"
#include "SimpleMarkerSymbol.h"
#include "SimpleLineSymbol.h"
#include "SimpleFillSymbol.h"
#include "SymbolLibraryRepository.h"

#include <memory>

BEGIN_NAME_SPACE(tGis, Core)

const char* const SimpleSymbolLibrary::_name = "SimpleSymbolLibrary";

SimpleSymbolLibrary* SimpleSymbolLibrary::_instance = nullptr;

SimpleSymbolLibrary & SimpleSymbolLibrary::INSTANCE()
{
	if (_instance == nullptr)
	{
		_instance = new SimpleSymbolLibrary();
		static std::unique_ptr<SimpleSymbolLibrary> shit(_instance);
	}

	return *_instance;
}

SimpleSymbolLibrary::SimpleSymbolLibrary()
{
	SymbolLibraryRepository::INSTANCE().AddSymbolLibrary(this);
}


SimpleSymbolLibrary::~SimpleSymbolLibrary()
{
}

const char * SimpleSymbolLibrary::GetName() const
{
	return _name;
}

int SimpleSymbolLibrary::GetSymbolCount() const
{
	return (SimpleMarkerSymbol::IdentifierEnd - SimpleMarkerSymbol::IdentifierBegin + 1)
		+ (SimpleLineSymbol::IdentifierEnd - SimpleLineSymbol::IdentifierBegin + 1)
		+ (SimpleFillSymbol::IdentifierEnd - SimpleFillSymbol::IdentifierBegin + 1);
}

ISymbol * SimpleSymbolLibrary::GetSymbol(int id) const
{
	if (id <= SimpleMarkerSymbol::IdentifierEnd)
		return new SimpleMarkerSymbol(id - SimpleMarkerSymbol::IdentifierBegin);
	else if(id <= SimpleLineSymbol::IdentifierEnd)
		return new SimpleLineSymbol(id - SimpleLineSymbol::IdentifierBegin);
	else if (id <= SimpleFillSymbol::IdentifierEnd)
		return new SimpleFillSymbol(id - SimpleFillSymbol::IdentifierBegin);

	return nullptr;
}

ISymbol * SimpleSymbolLibrary::GetSymbol(int id, int *nextId) const
{
	if (id <= SimpleMarkerSymbol::IdentifierEnd)
	{
		if (nextId != nullptr && id == SimpleMarkerSymbol::IdentifierEnd)
		{
			*nextId = SimpleLineSymbol::IdentifierBegin;
		}
		return new SimpleMarkerSymbol(id - SimpleMarkerSymbol::IdentifierBegin);
	}
	else if (id <= SimpleLineSymbol::IdentifierEnd)
	{
		if (nextId != nullptr && id == SimpleLineSymbol::IdentifierEnd)
		{
			*nextId = SimpleFillSymbol::IdentifierBegin;
		}
		return new SimpleLineSymbol(id - SimpleLineSymbol::IdentifierBegin);
	}
	else if (id <= SimpleFillSymbol::IdentifierEnd)
	{
		if (nextId != nullptr && id == SimpleFillSymbol::IdentifierEnd)
		{
			*nextId = -1;
		}
		return new SimpleFillSymbol(id - SimpleFillSymbol::IdentifierBegin);
	}
	return nullptr;
}

void SimpleSymbolLibrary::RevertSymbol(ISymbol * sym) const
{
	delete sym;
}

END_NAME_SPACE(tGis, Core)


