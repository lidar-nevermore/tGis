#include "VirtualDataSource.h"
#include <string>


BEGIN_NAME_SPACE(tGis, Core)

const char* const VirtualDataSource::_type = "F5038534-7EB7-4A9A-96B6-C84E9FEBD508";

const char * VirtualDataSource::GetType()
{
	return VirtualDataSource::_type;
}

const char * VirtualDataSource::S_GetType()
{
	return _type;
}

bool VirtualDataSource::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return false;
}


class VirtualDataSourceImpl
{
public:
	VirtualDataSourceImpl(VirtualDataSource* owner)
	{
		_owner = owner;
	}

	VirtualDataSource* _owner;
	std::string _name;
};



VirtualDataSource::VirtualDataSource(const char * name)
{
	_impl_ = new VirtualDataSourceImpl(this);
	_impl_->_name = name;
	Connect();
	_connected = true;
}

VirtualDataSource::~VirtualDataSource()
{
	delete _impl_;
}

END_NAME_SPACE(tGis, Core)


