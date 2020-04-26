#include "SimpleSymbolLibrary.h"
#include "SimpleMarkerSymbol.h"
#include "SimpleLineSymbol.h"
#include "SimpleFillSymbol.h"
#include "SymbolLibraryRepository.h"

#include <memory>

BEGIN_NAME_SPACE(tGis, Core)

SimpleSymbolLibrary* SimpleSymbolLibrary::_markerSymbolLibrary = nullptr;
SimpleSymbolLibrary * SimpleSymbolLibrary::GetMarkerSymbolLibrary()
{
	if (_markerSymbolLibrary == nullptr)
	{
		_markerSymbolLibrary = new SimpleSymbolLibrary(SymbolLibraryType::Marker);
		SymbolLibraryRepository::INSTANCE()->AddSymbolLibrary(_markerSymbolLibrary);
		//static std::unique_ptr<SimpleSymbolLibrary> shit(_markerSymbolLibrary);
	}

	return _markerSymbolLibrary;
}

SimpleSymbolLibrary* SimpleSymbolLibrary::_lineSymbolLibrary = nullptr;
SimpleSymbolLibrary * SimpleSymbolLibrary::GetLineSymbolLibrary()
{
	if (_lineSymbolLibrary == nullptr)
	{
		_lineSymbolLibrary = new SimpleSymbolLibrary(SymbolLibraryType::Line);
		SymbolLibraryRepository::INSTANCE()->AddSymbolLibrary(_lineSymbolLibrary);
		//static std::unique_ptr<SimpleSymbolLibrary> shit(_lineSymbolLibrary);
	}

	return _lineSymbolLibrary;
}

SimpleSymbolLibrary* SimpleSymbolLibrary::_fillSymbolLibrary = nullptr;
SimpleSymbolLibrary * SimpleSymbolLibrary::GetFillSymbolLibrary()
{
	if (_fillSymbolLibrary == nullptr)
	{
		_fillSymbolLibrary = new SimpleSymbolLibrary(SymbolLibraryType::Fill);
		SymbolLibraryRepository::INSTANCE()->AddSymbolLibrary(_fillSymbolLibrary);
		//static std::unique_ptr<SimpleSymbolLibrary> shit(_fillSymbolLibrary);
	}

	return _fillSymbolLibrary;
}

SimpleSymbolLibrary::SimpleSymbolLibrary(SymbolLibraryType libType)
{
	_libType = libType;
	switch (_libType)
	{
	case tGis::Core::Marker:
		_name = "SimpleMarkerSymbolLibrary";
		break;
	case tGis::Core::Line:
		_name = "SimpleLineSymbolLibrary";
		break;
	case tGis::Core::Fill:
		_name = "SimpleFileSymbolLibrary";
		break;
	default:
		break;
	}
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
	switch (_libType)
	{
	case tGis::Core::Marker:
		return SimpleMarkerSymbol::MaxId + 1;
	case tGis::Core::Line:
		return SimpleLineSymbol::MaxId + 1;
	default: //tGis::Core::Fill
		return SimpleFillSymbol::MaxId + 1;
	}
}

ISymbol * SimpleSymbolLibrary::GetSymbol(int id) const
{
	switch (_libType)
	{
	case tGis::Core::Marker:
		return id <= SimpleMarkerSymbol::MaxId? new SimpleMarkerSymbol(id) : nullptr;
	case tGis::Core::Line:
		return id <= SimpleLineSymbol::MaxId ? new SimpleLineSymbol(id) : nullptr;
	default: //tGis::Core::Fill
		return id <= SimpleFillSymbol::MaxId ? new SimpleFillSymbol(id) : nullptr;
	}
}

ISymbol * SimpleSymbolLibrary::GetSymbol(int id, int *nextId) const
{
	if (_libType == SymbolLibraryType::Marker)
	{
		if (id <= SimpleMarkerSymbol::MaxId)
		{
			if (nextId != nullptr)
			{
				if (id == SimpleMarkerSymbol::MaxId)
					*nextId = -1;
				else
					*nextId = id + 1;
			}
			return new SimpleMarkerSymbol(id);
		}
	}
	else if (_libType == SymbolLibraryType::Line)
	{
		if (id <= SimpleLineSymbol::MaxId)
		{
			if (nextId != nullptr)
			{
				if (id == SimpleLineSymbol::MaxId)
					*nextId = -1;
				else
					*nextId = id + 1;
			}
			return new SimpleLineSymbol(id);
		}
	}
	else
	{
		if (id <= SimpleFillSymbol::MaxId)
		{
			if (nextId != nullptr)
			{
				if (id == SimpleFillSymbol::MaxId)
					*nextId = -1;
				else
					*nextId = id + 1;
			}
			return new SimpleFillSymbol(id);
		}
	}

	if (nextId != nullptr)
		*nextId = -1;

	return nullptr;
}

void SimpleSymbolLibrary::RevertSymbol(ISymbol * sym) const
{
	delete sym;
}

END_NAME_SPACE(tGis, Core)


