#include "SimpleSymbolLibrary.h"
#include "SymbolLibraryRepository.h"
#include "SimpleSymbolLibrary.h"
#include <memory>

#include <vector>
#include <map>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

SymbolLibraryRepository* SymbolLibraryRepository::_instance = nullptr;


SymbolLibraryRepository* SymbolLibraryRepository::INSTANCE()
{
	if (_instance == nullptr)
	{
		_instance = new SymbolLibraryRepository();
		_instance->AddSymbolLibrary(SimpleSymbolLibrary::GetMarkerSymbolLibrary());
		_instance->AddSymbolLibrary(SimpleSymbolLibrary::GetLineSymbolLibrary());
		_instance->AddSymbolLibrary(SimpleSymbolLibrary::GetFillSymbolLibrary());
		static unique_ptr<SymbolLibraryRepository> shit(_instance);
	}

	return _instance;
}

struct c_str_ptr_less
{
	bool operator () (const char* a, const char* b) const
	{
		return std::strcmp(a, b) < 0;
	}
};


class SymbolLibraryRepositoryImpl
{
public:
	SymbolLibraryRepositoryImpl(SymbolLibraryRepository* owner)
	{
		_owner = owner;
	}

	SymbolLibraryRepository* _owner;

	vector<ISymbolLibrary*> _vecMarkerSymbolLibrary;
	vector<ISymbolLibrary*> _vecLineSymbolLibrary;
	vector<ISymbolLibrary*> _vecFillSymbolLibrary;
	map<const char*, ISymbolLibrary*, c_str_ptr_less> _mapSymbolLibrary;
};


SymbolLibraryRepository::SymbolLibraryRepository()
{
	_impl_ = new SymbolLibraryRepositoryImpl(this);
}

SymbolLibraryRepository::~SymbolLibraryRepository()
{
	for (vector<ISymbolLibrary*>::iterator it = _impl_->_vecMarkerSymbolLibrary.begin(); it != _impl_->_vecMarkerSymbolLibrary.end(); it++)
	{
		ISymbolLibrary* sl = *it;

		if (sl->_is_in_heap)
			delete sl;
	}

	for (vector<ISymbolLibrary*>::iterator it = _impl_->_vecLineSymbolLibrary.begin(); it != _impl_->_vecLineSymbolLibrary.end(); it++)
	{
		ISymbolLibrary* sl = *it;

		if (sl->_is_in_heap)
			delete sl;
	}

	for (vector<ISymbolLibrary*>::iterator it = _impl_->_vecFillSymbolLibrary.begin(); it != _impl_->_vecFillSymbolLibrary.end(); it++)
	{
		ISymbolLibrary* sl = *it;

		if (sl->_is_in_heap)
			delete sl;
	}

	delete _impl_;
}


size_t SymbolLibraryRepository::GetMarkerSymbolLibraryCount()
{
	return _impl_->_vecMarkerSymbolLibrary.size();
}

ISymbolLibrary * SymbolLibraryRepository::GetMarkerSymbolLibrary(size_t pos)
{
	return _impl_->_vecMarkerSymbolLibrary.at(pos);
}

size_t SymbolLibraryRepository::GetLineSymbolLibraryCount()
{
	return _impl_->_vecLineSymbolLibrary.size();
}

ISymbolLibrary * SymbolLibraryRepository::GetLineSymbolLibrary(size_t pos)
{
	return _impl_->_vecLineSymbolLibrary.at(pos);
}

size_t SymbolLibraryRepository::GetFillSymbolLibraryCount()
{
	return _impl_->_vecFillSymbolLibrary.size();
}

ISymbolLibrary * SymbolLibraryRepository::GetFillSymbolLibrary(size_t pos)
{
	return _impl_->_vecFillSymbolLibrary.at(pos);
}

void SymbolLibraryRepository::AddSymbolLibrary(ISymbolLibrary * sl)
{
	string name = sl->GetName();
	switch (sl->GetType())
	{
	case tGis::Core::Marker:
		_impl_->_vecMarkerSymbolLibrary.push_back(sl);
		break;
	case tGis::Core::Line:
		_impl_->_vecLineSymbolLibrary.push_back(sl);
		break;
	default: //tGis::Core::Fill
		_impl_->_vecFillSymbolLibrary.push_back(sl);
		break;
	}
	_impl_->_mapSymbolLibrary.insert(map<const char*, ISymbolLibrary*>::value_type(sl->GetName(), sl));
}

ISymbolLibrary * SymbolLibraryRepository::GetSymbolLibrary(const char * symLibName)
{
	map<const char*, ISymbolLibrary*>::iterator pos = _impl_->_mapSymbolLibrary.find(symLibName);

	if (pos != _impl_->_mapSymbolLibrary.end())
		return (*pos).second;
	return nullptr;
}


END_NAME_SPACE(tGis, Core)


