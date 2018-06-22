#pragma once

#ifndef __EVENT_H__
#define __EVENT_H__

#include "Helper.h"
#include "ITGisObject.h"

using namespace std;

BEGIN_NAME_SPACE(tGis, Core)

template<typename ...Args>
struct IEventHandler
{
	virtual void operator()(Args&... args) = 0;

	IEventHandler() {};
	virtual ~IEventHandler() {};

private:
	IEventHandler(const IEventHandler &) = delete;
	IEventHandler &operator=(const IEventHandler &) = delete;
};

template<typename T, typename ...Args>
class EventHandler : public IEventHandler<Args...>
{
public:
	typedef void(T::*Handler)(Args...);
	EventHandler(T* receiver, Handler handler)
	{
		_receiver = receiver;
		_handler = handler;
	}

public:
	void operator()(Args&... args)
	{
		(_receiver->*_handler)(args...);
	}

private:
	Handler _handler;
	T* _receiver;
};

template<typename Callable, typename ...Args>
class CallableEventHandler : public IEventHandler<Args...>
{
public:
	CallableEventHandler(Callable handler)
	{
		_handler = handler;
	}

public:
	void operator()(Args&... args)
	{
		_handler(args...);
	}

private:
	Callable _handler;
};

template<typename ...Args>
class Event
{
public:
	typedef IEventHandler<Args...>* Callable;

public:
	Event() 
	{
		_valid = false;
		_event = nullptr;
	};

	Event(const Callable& h)
	{
		_handler = h;
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

	const Event<Args...>& operator = (const Callable& h)
	{
		_handler = h;
		_valid = true;
		if (_event != nullptr)
		{
			delete _event;
			_event = nullptr;
		}

		return *this;
	}

	Event(const Event<Args...> & e)
	{
		this->Add(e);
	}

	Event<Args...> &operator=(const Event<Args...> & e)
	{
		_valid = false;
		if (_event != nullptr)
		{
			delete _event;
			_event = nullptr;
		}

		this->Add(e);

		return *this;
	}

public:
	void Raise(Args&... args)
	{
		if (_valid)
		{
			(*_handler)(args...);
		}
		if (_event != nullptr)
		{
			_event->Raise(args...);
		}
	}

	void operator()(Args&... args)
	{
		this->Raise(args...);
	}

public:
	void Add(const Callable& h)
	{
		if (_valid)
		{
			if (_event != nullptr)
			{
				_event->Add(h);
			}
			else
			{
				_event = new Event<Args...>(h);
			}
		}
		else
		{
			_handler = h;
			_valid = true;
		}
	}

	void Remove(const Callable& h)
	{
		if (_valid && _handler == h)
		{
			if (_event == nullptr)
			{
				_valid = false;
			}
			else
			{
				Event<Args...>* event_ = _event;
				_valid = _event->_valid;
				_handler = _event->_handler;
				_event = _event->_event;

				event_->_event = nullptr;
				delete event_;				
			}
		}
		else if (_event != nullptr)
		{
			_event->Remove(h);
		}
	}

	void Add(const Event<Args...>& e)
	{
		Event<Args...>* event_ = const_cast<Event<Args...>*>(&e);
		while (event_ != nullptr)
		{
			if (event_->_valid)
			{
				this->Add(event_->_handler);
			}
			event_ = event_->_event;
		}
	}

	void Remove(const Event<Args...>& e)
	{
		Event<Args...>* event_ = const_cast<Event<Args...>*>(&e);
		while (event_ != nullptr)
		{
			if (event_->_valid)
			{
				this->Remove(event_->_handler);
			}
			event_ = event_->_event;
		}
	}

public:

	const Event<Args...>& operator += (const Callable& h)
	{
		this->Add(h);

		return *this;
	}

	const Event<Args...>& operator -= (const Callable& h)
	{
		this->Remove(h);

		return *this;
	}

	const Event<Args...>& operator += (const Event<Args...>& e)
	{
		this->Add(e);

		return *this;
	}

	const Event<Args...>& operator -= (const Event<Args...>& e)
	{
		this->Remove(e);

		return *this;
	}

private:
	bool _valid;
	Callable _handler;
	Event<Args...>* _event;
};

END_NAME_SPACE(tGis, Core)


#endif


