#include "wxTGisApplication.h"
#include "wx/process.h"

#include <string>
#include <vector>

using namespace std;


BEGIN_NAME_SPACE(tGis, Gui)

class MyProcess;

class wxTGisApplicationImpl
{
public:
	wxTGisApplicationImpl(wxTGisApplication* owner)
	{
		_owner = owner;
	}

	wxTGisApplication* _owner;

	vector<MyProcess*> _vecProcess;
};


class MyProcess : public wxProcess
{
public:
	MyProcess(wxTGisApplicationImpl *owner)
		: wxProcess(nullptr)
	{
		_owner = owner;
	}

	// instead of overriding this virtual function we might as well process the
	// event from it in the frame class - this might be more convenient in some
	// cases
	virtual void OnTerminate(int pid, int status) wxOVERRIDE
	{
		for (auto it = _owner->_vecProcess.begin(); it != _owner->_vecProcess.end(); it++)
		{
			MyProcess* process = *it;
			if (process == this)
			{
				_owner->_vecProcess.erase(it);
				delete this;				
				break;
			}
		}
	}

protected:
	wxTGisApplicationImpl *_owner;
};


wxTGisApplication::wxTGisApplication()
{
	_impl_ = new wxTGisApplicationImpl(this);
}


wxTGisApplication::~wxTGisApplication()
{
	delete _impl_;
}


void wxTGisApplication::Execute(const char * cmd)
{
	MyProcess * const process = new MyProcess(_impl_);
	long pid = wxExecute(cmd, wxEXEC_ASYNC, process);
	if (!pid)
	{
		delete process;
	}
	else
	{
		_impl_->_vecProcess.push_back(process);
	}
}

END_NAME_SPACE(tGis, Gui)