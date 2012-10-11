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
#include <unordered_map>
#include <vector>

namespace PZ
{
	class HandlerFunctionBase
	{
	public:
		virtual ~HandlerFunctionBase()
		{}

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
			(instance->*fn)(static_cast<const EventT&>(e));
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

		template<class T, class EventT>
		bool Subscribe(EventType type, EventHandler *handler, void (T::*memFn)(EventT&), T *obj = nullptr)
		{
			if (obj == nullptr)
			{
				obj = dynamic_cast<T*>(handler);
				if (obj == nullptr)
					return false;
			}

			TypeInfo typeInfo(typeid(EventT));

			HandlerFunctionList &list = subscribers[typeInfo][type];
			if (!HasHandler(list, handler))
			{
				list.push_back(std::make_pair(handler, new MemberFunctionHandler<T, EventT>(obj, memFn)));

				handler->SubscribeTo(this, typeInfo, type);

				return true;
			}

			return false;
		}
		template<class T, class EventT>
		bool Unsubscribe(EventType type, EventHandler *handler, void (T::*memFn)(EventT&))
		{
			TypeInfo typeInfo(typeid(EventT));
			HandlerFunctionList &list = subscribers[typeInfo][type];
			for (HandlerFunctionList::iterator it = list.begin(); it != list.end(); ++it)
			{
				if ((*it).first == handler)
				{
					delete (*it).second;
					list.erase(it);
					handler->UnsubscribeTo(this, typeInfo, type);
					return true;
				}
			}

			return false;
		}

		void EmitEvent(const Event &e) const;

	private:
		void SubscribeTo(EventHandler *handler, const TypeInfo &typeInfo, EventType type);
		void UnsubscribeTo(EventHandler *handler, const TypeInfo &typeInfo, EventType type);

		void Unsubscribe(EventType type, EventHandler *handler, const TypeInfo &typeInfo);

		typedef std::vector<std::pair<EventHandler*, HandlerFunctionBase*>> HandlerFunctionList;
		typedef std::unordered_map<EventType, HandlerFunctionList> EventHandlerMap;

		bool HasHandler(HandlerFunctionList &list, EventHandler *handler) const;

		typedef std::map<TypeInfo, EventHandlerMap> SubscriberMap;
		SubscriberMap subscribers;

		typedef std::pair<EventHandler*, std::pair<TypeInfo, EventType>> Subscription;
		typedef std::vector<Subscription> SubscriptionList;
		SubscriptionList subscriptions;
	};
}

#endif // PZ_EventHandler_h__
