#include "SimpleSymbolLibrary.h"
#include "SimpleMarkerSymbol.h"
#include "SimpleLineSymbol.h"
#include "SimpleFillSymbol.h"
#include "SymbolLibraryRepository.h"

#include <memory>
#include <assert.h>

BEGIN_NAME_SPACE(tGis, Core)

SimpleSymbolLibrary* SimpleSymbolLibrary::_markerSymbolLibrary = nullptr;
SimpleSymbolLibrary * SimpleSymbolLibrary::GetMarkerSymbolLibrary()
{
	if (_markerSymbolLibrary == nullptr)
	{
		_markerSymbolLibrary = new SimpleSymbolLibrary(SymbolLibraryType::Marker);
		//调用一个无副作用的函数，确保_markerSymbolLibrary放到Repository中了
		SymbolLibraryRepository::INSTANCE()->GetMarkerSymbolLibraryCount();
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
		SymbolLibraryRepository::INSTANCE()->GetMarkerSymbolLibraryCount();
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
		SymbolLibraryRepository::INSTANCE()->GetMarkerSymbolLibraryCount();
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
		_name = "SimpleFillSymbolLibrary";
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

bool SimpleSymbolLibrary::SymbolExists(int id) const
{
	if (id < 0)
		return false;

	switch (_libType)
	{
	case tGis::Core::Marker:
		return id <= SimpleMarkerSymbol::MaxId;
	case tGis::Core::Line:
		return id <= SimpleLineSymbol::MaxId;
	default: //tGis::Core::Fill
		return id <= SimpleFillSymbol::MaxId;
	}
}

ISymbol * SimpleSymbolLibrary::GetSymbol(int id) const
{
	if (id < 0)
		return nullptr;

	switch (_libType)
	{
	case tGis::Core::Marker:
		return id <= SimpleMarkerSymbol::MaxId? new SimpleMarkerSymbol(id, this) : nullptr;
	case tGis::Core::Line:
		return id <= SimpleLineSymbol::MaxId ? new SimpleLineSymbol(id, this) : nullptr;
	default: //tGis::Core::Fill
		return id <= SimpleFillSymbol::MaxId ? new SimpleFillSymbol(id, this) : nullptr;
	}
}

bool SimpleSymbolLibrary::SymbolExists(int id, int * nextId) const
{
	assert(id >= 0);

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
			return true;
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
			return true;
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
			return true;
		}
	}

	if (nextId != nullptr)
		*nextId = -1;

	return false;
}

ISymbol * SimpleSymbolLibrary::GetSymbol(int id, int *nextId) const
{
	assert(id >= 0);

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
			return new SimpleMarkerSymbol(id, this);
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
			return new SimpleLineSymbol(id, this);
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
			return new SimpleFillSymbol(id, this);
		}
	}

	if (nextId != nullptr)
		*nextId = -1;

	return nullptr;
}


END_NAME_SPACE(tGis, Core)


