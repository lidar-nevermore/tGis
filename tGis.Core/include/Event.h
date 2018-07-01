#pragma once

#ifndef __EVENT_H__
#define __EVENT_H__

#include "Helper.h"
#include "EventBase.h"
#include "elr_mpl.h"


BEGIN_NAME_SPACE(tGis, Core)

template<typename ...Args>
struct IEventHandler
{
	virtual void operator()(Args&... args) = 0;

	IEventHandler() {};
	virtual ~IEventHandler() {};

	virtual bool IsEqual(IEventHandler<Args...>* handler)
	{
		return this == handler;
	}

private:
	IEventHandler(const IEventHandler &) = delete;
	IEventHandler &operator=(const IEventHandler &) = delete;
};

template<typename T, typename ...Args>
class MemberEventHandler : public IEventHandler<Args...>
{
public:
	typedef void(T::*Handler)(Args...);
	MemberEventHandler(T* receiver, Handler handler)
	{
		_receiver = receiver;
		_handler = handler;
	}

	virtual ~MemberEventHandler() {};

	virtual bool IsEqual(const IEventHandler<Args...>* handler)
	{
		if (this == handler)
			return true;

		MemberEventHandler<T, Args...>* h = dynamic_cast<MemberEventHandler<T, Args...>*>(const_cast<IEventHandler<Args...>*>(handler));

		if (h == nullptr)
			return false;

		if (h->_receiver == _receiver && h->_handler == _handler)
			return true;

		return false;
	}

	static void* operator new(size_t size)
	{
		void* p = elr_mpl_alloc_multi(NULL, size);
		if (p == NULL)
		{
			throw std::bad_alloc();
		}

		return p;
	}

	static void operator delete(void *p)
	{
		elr_mpl_free(p);
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
	CallableEventHandler(const Callable& handler)
	{
		_handler = handler;
	}

	virtual ~CallableEventHandler() {};

	virtual bool IsEqual(const IEventHandler<Args...>* handler)
	{
		if (this == handler)
			return true;

		CallableEventHandler<Callable, Args...>* h = dynamic_cast<CallableEventHandler<Callable, Args...>*>(const_cast<IEventHandler<Args...>*>(handler));

		if (h == nullptr)
			return false;

		if (h->_handler == _handler)
			return true;

		return false;
	}


	static void* operator new(size_t size)
	{
		void* p = elr_mpl_alloc_multi(NULL, size);
		if (p == NULL)
		{
			throw std::bad_alloc();
		}

		return p;
	}

	static void operator delete(void *p)
	{
		elr_mpl_free(p);
	}

public:
	void operator()(Args&... args)
	{
		_handler(args...);
	}

private:
	const Callable _handler;
};

template<typename ...Args>
class Event : public EventBase
{
public:
	typedef IEventHandler<Args...>* EventHandler;

public:
	Event() 
	{
		_valid = false;
		_event = nullptr;
		_handler = nullptr;
		_isInternalHandler = false;
	};

	Event(const EventHandler& h)
	{
		_handler = h;
		_valid = true;
		_event = nullptr;
		_isInternalHandler = false;
	}

	~Event() 
	{
		if (_valid && _isInternalHandler)
		{
			delete _handler;
		}

		if (_event != nullptr) 
		{
			delete _event;
		}
	};

	const Event<Args...>& operator = (const EventHandler& h)
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

	void Raise(Args&&... args)
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

	void operator()(Args&&... args)
	{
		this->Raise(args...);
	}

private:
	void Add(const EventHandler& h, bool isInternalHandler)
	{
		if (_valid)
		{
			if (_event != nullptr)
			{
				_event->Add(h, isInternalHandler);
			}
			else
			{
				_event = new Event<Args...>(h);
				_event->_isInternalHandler = isInternalHandler;
			}
		}
		else
		{
			_handler = h;
			_valid = true;
			_isInternalHandler = isInternalHandler;
		}
	}

public:
	void Add(const EventHandler& h)
	{
		this->Add(h, false);
	}

	void Remove(const EventHandler& h)
	{
		if (_valid && _handler->IsEqual(h))
		{
			if (_isInternalHandler)
				delete _handler;

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
				event_->_valid = false;
				delete event_;				
			}
		}
		else if (_event != nullptr)
		{
			_event->Remove(h);
		}
	}

	template<typename T>
	void Add(T* receiver, void(T::*h)(Args...))
	{
		MemberEventHandler<T, Args...>* handler = new MemberEventHandler<T, Args...>(receiver, h);
		this->Add(handler, true);
	}

	template<typename T>
	void Remove(T* receiver, void(T::*h)(Args...))
	{
		MemberEventHandler<T, Args...> handler(receiver, h);
		this->Remove(&handler);
	}

	template<typename Callable>
	void Add(const Callable& h)
	{
		CallableEventHandler<Callable, Args...>* handler = new CallableEventHandler<Callable, Args...>(h);
		this->Add(handler, true);
	}

	template<typename Callable>
	void Remove(const Callable& h)
	{
		CallableEventHandler<Callable, Args...> handler(h);
		this->Remove(&handler);
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

	const Event<Args...>& operator += (const EventHandler& h)
	{
		this->Add(h);

		return *this;
	}

	const Event<Args...>& operator -= (const EventHandler& h)
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
	EventHandler _handler;
	bool _isInternalHandler;
	Event<Args...>* _event;
};

struct TGIS_API Progress
{
	Progress(int value, int max = 100, const char* msg = nullptr)
		:Value(value)
		, Max(max)
		, Message(msg)
	{
	}

	const int Value;
	const int Max;
	const char* const Message;
};

typedef IEventHandler<Progress> ProgressEventHandler;
typedef Event<Progress> ProgressEvent;

template struct TGIS_API IEventHandler<Progress>;
template class TGIS_API Event<Progress>;


END_NAME_SPACE(tGis, Core)


#endif


