#pragma once

#ifndef __EVENT_H__
#define __EVENT_H__

#include "Helper.h"
#include "ITGisObject.h"

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

#define MAKE_ARGS(...) __VA_ARGS__

#define MAKE_EVENT(HANDLER, ...)

template<typename Handler, typename ...Args>
class Event
{
public:
	Event(const Event &) = delete;
	Event &operator=(const Event &) = delete;

public:
	Event() 
	{
		_valid = false;
		_event = nullptr;
	};

	Event(const Handler& h)
	{
		_hander = h;
		_valid = true;
		_event = nullptr;
	}

	~Event() 
	{
		if (_event != nullptr) 
		{
			delete _event;
		}
	};

public:
	void Add(Handler& h)
	{
		if (_event != nullptr)
		{
			_event->Add(h);
		}
		else
		{
			if (_valid)
			{
				_event = new Event<Handler, Args...>(h);
			}
			else
			{
				_hander = h;
				_valid = true;
			}
		}
	}

	void Remove(Handler& h)
	{
		if (_hander == h)
		{
			if (_event == nullptr)
			{
				_valid = false;
			}
			else
			{
				_hander = _event->_hander;
				_event = _event->_event;
				_valid = _event->_valid;
			}
		}
		else
		{
			_event->Remove(h);
		}
	}

	void Raise(Args&&... args)
	{
		if (_valid)
		{
			_hander(args...);
			if (_event != nullptr)
				_event->Raise(std::forward<Args>(args)...);
		}
	}

	const Event<Handler, Args...>& operator = (const Handler& h)
	{
		_hander = h;
		_valid = true;
		if (_event != nullptr)
		{
			delete _event;
			_event = nullptr;
		}

		return *this;
	}

	//const Event<Handler, Args...>& operator += (const Handler&& h)
	//{
	//	this->Add(h);

	//	return *this;
	//}

	//const Event<Handler, Args...>& operator -= (const Handler& h)
	//{
	//	this->Remove(h);

	//	return *this;
	//}

	void operator()(Args&&... args)
	{
		this->Raise(std::forward<Args>(args)...);
	}

private:
	bool _valid;
	Handler _hander;
	Event<Handler, Args...>* _event;
};

END_NAME_SPACE(tGis, Core)


#endif


