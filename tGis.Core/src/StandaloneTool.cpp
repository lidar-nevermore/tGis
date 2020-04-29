#include "StandaloneTool.h"
#include <vector>
#include <string>
#include <sstream>
#include "tGisApplication.h"

#include <stdlib.h>

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)


const char* const StandaloneTool::_type = "25DDF5CC-0351-4AF1-BE7D-89351FAFDA78";


const char * StandaloneTool::GetType()
{
	return _type;
}

bool StandaloneTool::IsTypeOf(const char * type)
{
	if (strcmp(type, _type) == 0)
		return true;
	return false;
}

const char * StandaloneTool::S_GetType()
{
	return _type;
}

class StandaloneToolImpl
{
public:
	StandaloneToolImpl(StandaloneTool* owner)
	{
		_owner = owner;
	}

	StandaloneTool* _owner;

	string _name;
	string _exe;
	string _absExe;
	vector<string> _vecParam;
};

StandaloneTool::StandaloneTool(const char* name)
{
	_impl_ = new StandaloneToolImpl(this);
	_impl_->_name = name;
}

StandaloneTool::StandaloneTool(const StandaloneTool & tool)
{
	_impl_ = new StandaloneToolImpl(this);
	operator=(tool);
}

StandaloneTool & StandaloneTool::operator=(const StandaloneTool & tool)
{
	_impl_->_name = tool._impl_->_name;
	_impl_->_exe = tool._impl_->_exe;
	_impl_->_absExe = tool._impl_->_absExe;
	_impl_->_vecParam.insert(_impl_->_vecParam.end(),
		tool._impl_->_vecParam.begin(),
		tool._impl_->_vecParam.end());

	return *this;
}


StandaloneTool::~StandaloneTool()
{
	delete _impl_;
}

const char * StandaloneTool::GetName()
{
	return _impl_->_name.c_str();
}

void StandaloneTool::Execute()
{
	//TODO: 无命令行窗口方式的启动外部程序
	std::ostringstream oss;
	oss << "\"";
	oss << _impl_->_absExe;
	oss << "\" ";

	for (auto it = _impl_->_vecParam.begin(); it != _impl_->_vecParam.begin(); it++)
	{
		oss << *it;
		oss << " ";
	}

	string cmdstr = oss.str();

	tGisApplication::INSTANCE()->Execute(cmdstr.c_str());
	//system(cmdstr.c_str());
}

void StandaloneTool::SetExeFile(const char * exe)
{
	_impl_->_exe = exe;
	//如果以
	if (_impl_->_exe.compare(0, 7, "$(tGis)") == 0)
	{
		_impl_->_absExe = tGisApplication::INSTANCE()->GetExeDir();
		//_impl_->_absExe.append(TGIS_PATH_SEPARATOR_STR);
		_impl_->_absExe.append(_impl_->_exe.substr(7));
	}
	else
	{
		_impl_->_absExe = exe;
	}
}

const char * StandaloneTool::GetExeFile()
{
	return _impl_->_exe.c_str();
}

size_t StandaloneTool::GetParamCount()
{
	return _impl_->_vecParam.size();
}

const char * StandaloneTool::GetParam(size_t pos)
{
	return _impl_->_vecParam.at(pos).c_str();
}

void StandaloneTool::AddParam(const char * param)
{
	_impl_->_vecParam.push_back(param);
}

void StandaloneTool::RemoveParam(size_t pos)
{
	vector<string>::iterator it = _impl_->_vecParam.begin() + pos;
	_impl_->_vecParam.erase(it);
}

void StandaloneTool::ClearParam()
{
	_impl_->_vecParam.clear();
}


END_NAME_SPACE(tGis, Core)

