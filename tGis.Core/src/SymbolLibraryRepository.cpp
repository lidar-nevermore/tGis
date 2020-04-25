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

	vector<ISymbolLibrary*> _vecSymbolLibrary;
	map<const char*, ISymbolLibrary*, c_str_ptr_less> _mapSymbolLibrary;
};


SymbolLibraryRepository::SymbolLibraryRepository()
{
	_impl_ = new SymbolLibraryRepositoryImpl(this);
}


SymbolLibraryRepository::~SymbolLibraryRepository()
{
	for (vector<ISymbolLibrary*>::iterator it = _impl_->_vecSymbolLibrary.begin(); it != _impl_->_vecSymbolLibrary.end(); it++)
	{
		ISymbolLibrary* sl = *it;

		if (sl->_is_in_heap)
			delete sl;
	}

	delete _impl_;
}

size_t SymbolLibraryRepository::GetSymbolLibraryCount()
{
	return _impl_->_vecSymbolLibrary.size();
}

ISymbolLibrary * SymbolLibraryRepository::GetSymbolLibrary(size_t pos)
{
	return _impl_->_vecSymbolLibrary.at(pos);
}

void SymbolLibraryRepository::AddSymbolLibrary(ISymbolLibrary * sl)
{
	string name = sl->GetName();
	_impl_->_vecSymbolLibrary.push_back(sl);
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


