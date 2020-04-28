#include "StandaloneTool.h"
#include <vector>
#include <string>
#include "tGisApplication.h"

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

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

StandaloneTool::StandaloneTool(const char* name, ToolKit* parent)
	: ITool(parent)
{
	_impl_ = new StandaloneToolImpl(this);
	_impl_->_name = name;
}


StandaloneTool::~StandaloneTool()
{
	delete _impl_;
}

void StandaloneTool::SetExeFile(const char * exe)
{
	_impl_->_exe = exe;
	//Èç¹ûÒÔ
	if (_impl_->_exe.compare(0, 7, "$(tGis)") == 0)
	{
		_impl_->_absExe = tGisApplication::INSTANCE()->GetExeDir();
		//_impl_->_absExe.append(TGIS_PATH_SEPARATOR_STR);
		_impl_->_absExe.append(_impl_->_exe.substr(8));
	}
	else
	{
		_impl_->_absExe = exe;
	}
}

const char * StandaloneTool::GetExeFile()
{
	return _impl_->_absExe.c_str();
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

