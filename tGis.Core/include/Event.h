#pragma once

#ifndef __EVENT_H__
#define __EVENT_H__

#include "Helper.h"
#include "EventBase.h"
#include "elr_mpl.h"
#include <functional>

BEGIN_NAME_SPACE(tGis, Core)

template<typename ...Args>
struct IEventHandler
{
	virtual void operator()(Args&... args) = 0;
	virtual void operator()(const Args&... args) = 0;
	//virtual void operator()(Args&&... args) = 0;
	//virtual void operator()(const Args&&... args) = 0;

	virtual bool IsEqual(const IEventHandler<Args...>* handler)
	{
		return this == handler;
	}

	IEventHandler() {};
	virtual ~IEventHandler() {};

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

	static void* operator new[](size_t size)
	{
		void* p = elr_mpl_alloc_multi(NULL, size);
		if (p == NULL)
		{
			throw std::bad_alloc();
		}

		return p;
	}

	static void operator delete[](void *p)
	{
		elr_mpl_free(p);
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
	MemberEventHandler(T* receiver,const Handler handler)
		:_receiver(receiver),
		_handler(handler)
	{
	}

	virtual ~MemberEventHandler() {};

	virtual bool IsEqual(const IEventHandler<Args...>* handler) override
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

public:
	void operator()(Args&... args)
	{
		(_receiver->*_handler)(args...);
	}

	void operator()(const Args&... args)
	{
		(_receiver->*_handler)(args...);
	}

	//void operator()(Args&&... args)
	//{
	//	(_receiver->*_handler)(args...);
	//}

	//void operator()(const Args&&... args)
	//{
	//	(_receiver->*_handler)(args...);
	//}

private:
	const Handler _handler;
	T* _receiver;

private:
	MemberEventHandler(const MemberEventHandler &) = delete;
	MemberEventHandler &operator=(const MemberEventHandler &) = delete;
};

template<typename ...Args>
class FunctionEventHandler : public IEventHandler<Args...>
{
public:
	typedef void(__cdecl *Handler)(Args...);
	FunctionEventHandler(const Handler handler)
		:_handler(handler)
	{
	}

	virtual ~FunctionEventHandler() {};

	virtual bool IsEqual(const IEventHandler<Args...>* handler)
	{
		if (this == handler)
			return true;

		FunctionEventHandler<Args...>* h = dynamic_cast<FunctionEventHandler<Args...>*>(const_cast<IEventHandler<Args...>*>(handler));

		if (h == nullptr)
			return false;

		if (h->_handler == _handler)
			return true;

		return false;
	}

public:
	void operator()(Args&... args)
	{
		_handler(args...);
	}

	void operator()(const Args&... args)
	{
		_handler(args...);
	}

	//void operator()(Args&&... args)
	//{
	//	_handler(args...);
	//}

	//void operator()(const Args&&... args)
	//{
	//	_handler(args...);
	//}

private:
	const Handler _handler;

private:
	FunctionEventHandler(const FunctionEventHandler &) = delete;
	FunctionEventHandler &operator=(const FunctionEventHandler &) = delete;
};

template<typename ...Args>
class FunctorEventHandler : public IEventHandler<Args...>
{
public:
	FunctorEventHandler(const std::function<void(Args...)>* handler)
		:_handler(handler)
	{
	}

	virtual ~FunctorEventHandler() {};

	virtual bool IsEqual(const IEventHandler<Args...>* handler)
	{
		if (this == handler)
			return true;

		FunctorEventHandler<Args...>* h = dynamic_cast<FunctorEventHandler<Args...>*>(const_cast<IEventHandler<Args...>*>(handler));

		if (h == nullptr)
			return false;

		if (h->_handler == _handler)
			return true;

		return false;
	}

public:
	void operator()(Args&... args)
	{
		(*_handler)(args...);
	}

	void operator()(const Args&... args)
	{
		(*_handler)(args...);
	}

	//void operator()(Args&&... args)
	//{
	//	_handler(args...);
	//}

	//void operator()(const Args&&... args)
	//{
	//	_handler(args...);
	//}

private:
	const std::function<void(Args...)>* _handler;

private:
	FunctorEventHandler(const FunctorEventHandler &) = delete;
	FunctorEventHandler &operator=(const FunctorEventHandler &) = delete;
};

template<typename ...Args>
class Event : public EventBase
{
public:
	Event() 
	{
		_valid = false;
		_event = nullptr;
		_handler = nullptr;
		_isInternalHandler = false;
	};

	Event(IEventHandler<Args...>* h)
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

	const Event<Args...>& operator = (IEventHandler<Args...>* h)
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

	void Raise(const Args&... args)
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

	//void Raise(Args&&... args)
	//{
	//	if (_valid)
	//	{
	//		(*_handler)(args...);
	//	}
	//	if (_event != nullptr)
	//	{
	//		_event->Raise(args...);
	//	}
	//}

	//void Raise(const Args&&... args)
	//{
	//	if (_valid)
	//	{
	//		(*_handler)(args...);
	//	}
	//	if (_event != nullptr)
	//	{
	//		_event->Raise(args...);
	//	}
	//}

	void operator()(Args&... args)
	{
		this->Raise(args...);
	}

	void operator()(const Args&... args)
	{
		this->Raise(args...);
	}

	//void operator()(Args&&... args)
	//{
	//	this->Raise(args...);
	//}

	//void operator()(const Args&&... args)
	//{
	//	this->Raise(args...);
	//}

private:
	void Add(IEventHandler<Args...>* h, bool isInternalHandler)
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
	void Add(IEventHandler<Args...>* h)
	{
		this->Add(h, false);
	}

	void Remove(IEventHandler<Args...>* h)
	{
		if (_valid && _handler->IsEqual(h))
		{
			if (_isInternalHandler)
			{
				delete _handler;
				_handler = nullptr;
			}

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

				event_->_handler = nullptr;
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

	void Add(void(*h)(Args...))
	{
		FunctionEventHandler<Args...>* handler = new FunctionEventHandler<Args...>(h);
		this->Add(handler, true);
	}

	void Remove(void(*h)(Args...))
	{
		FunctionEventHandler<Args...> handler(h);
		this->Remove(&handler);
	}

	void Add(const std::function<void(Args...)>* h)
	{
		FunctorEventHandler<Args...>* handler = new FunctorEventHandler<Args...>(h);
		this->Add(handler, true);
	}

	void Remove(const std::function<void(Args...)>* h)
	{
		FunctorEventHandler<Args...> handler(h);
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

	const Event<Args...>& operator += (IEventHandler<Args...>* h)
	{
		this->Add(h);

		return *this;
	}

	const Event<Args...>& operator -= (IEventHandler<Args...>* h)
	{
		this->Remove(h);

		return *this;
	}

	const Event<Args...>& operator += (void(*h)(Args...))
	{
		this->Add(h);

		return *this;
	}

	const Event<Args...>& operator -= (void(*h)(Args...))
	{
		this->Remove(h);

		return *this;
	}

	const Event<Args...>& operator += (const std::function<void(Args...)>* h)
	{
		this->Add(h);

		return *this;
	}

	const Event<Args...>& operator += (const std::function<void(Args...)>& h)
	{
		this->Add(&h);

		return *this;
	}

	const Event<Args...>& operator -= (const std::function<void(Args...)>* h)
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
	IEventHandler<Args...>* _handler;
	//_handler是否是在本类的实例内部创建的
	//内部创建的_handler在移除时需要释放内存
	bool _isInternalHandler;
	Event<Args...>* _event;
};

END_NAME_SPACE(tGis, Core)


#endif


