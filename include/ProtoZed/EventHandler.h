/*
Copyright (c) 2012 Johannes Häggqvist

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#ifndef PZ_EventHandler_h__
#define PZ_EventHandler_h__

#include <ProtoZed/Event.h>
#include <ProtoZed/TypeInfo.h>

#include <map>
#include <vector>

namespace PZ
{
	class HandlerFunctionBase
	{
	public:
		virtual ~HandlerFunctionBase()
		{}

		void Exec(const Event &e)
		{
			Call(e);
		}

	private:
		virtual void Call(const Event &e) = 0;
	};
	template<class T, class EventT>
	class MemberFunctionHandler : public HandlerFunctionBase
	{
	public:
		typedef void (T::*MemberFunc)(EventT&);

		MemberFunctionHandler(T *instance, MemberFunc fn) : instance(instance), fn(fn)
		{}

		virtual void Call(const Event &e)
		{
			(instance->*fn)(static_cast<EventT&>(e));
		}

	private:
		T *instance;
		MemberFunc fn;
	};

	class EventHandler
	{
	public:
		EventHandler();
		virtual ~EventHandler();

		bool SubscribeTo(EventHandler *handler);
		bool UnsubscribeTo(EventHandler *handler);

		template<class T, class EventT>
		bool RegisterEvent(T *obj, void (T::*memFn)(EventT&))
		{
			return handlers.insert(std::make_pair(TypeInfo(typeid(EventT)), new MemberFunctionHandler<T, EventT>(obj, memFn))).second;
		}
		template<class T, class EventT>
		bool UnregisterEvent(T *obj, void (T::*memFn)(EventT&))
		{
			HandlerMap::iterator it = handlers.find(TypeInfo(typeid(EventT)));
			if (it != handlers.end())
			{
				delete (*it).second;
				handlers.erase(it);
				return true;
			}

			return false;
		}

		void HandleEvent(const Event &e);
		void EmitEvent(const Event &e) const;

	private:
		bool Subscribe(EventHandler *handler);
		bool Unsubscribe(EventHandler *handler);

		typedef std::map<TypeInfo, HandlerFunctionBase*> HandlerMap;
		HandlerMap handlers;

		typedef std::vector<EventHandler*> SubscriberList;
		SubscriberList subscribers;
		SubscriberList subscriptions;
	};
}

#endif // PZ_EventHandler_h__
