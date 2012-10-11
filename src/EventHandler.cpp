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
#include <ProtoZed/EventHandler.h>

#include <algorithm>

namespace PZ
{
	EventHandler::EventHandler()
	{
	}
	EventHandler::~EventHandler()
	{
		while (!subscriptions.empty())
		{
			Subscription &sub = subscriptions.front();
			sub.first->Unsubscribe(sub.second.second, this, sub.second.first);
		}

		for (SubscriberMap::iterator it = subscribers.begin(); it != subscribers.end(); ++it)
		{
			EventHandlerMap &handlerMap = (*it).second;
			for (EventHandlerMap::iterator it2 = handlerMap.begin(); it2 != handlerMap.end(); ++it2)
			{
				HandlerFunctionList &list = (*it2).second;
				for (HandlerFunctionList::iterator it3 = list.begin(); it3 != list.end(); ++it3)
				{
					(*it3).first->UnsubscribeTo(this, (*it).first, (*it2).first);
					delete (*it3).second;
					(*it3).second = nullptr;
				}
			}
		}
		subscribers.clear();
	}

	void EventHandler::EmitEvent(const Event &e) const
	{
		SubscriberMap::const_iterator it1 = subscribers.find(TypeInfo(typeid(e)));
		if (it1 == subscribers.cend())
			return;

		std::unordered_map<EventType, HandlerFunctionList>::const_iterator it2 = (*it1).second.find(e.GetType());
		if (it2 == (*it1).second.cend())
			return;

		const HandlerFunctionList &list = (*it2).second;
		for (HandlerFunctionList::const_iterator it = list.cbegin(); it != list.cend(); ++it)
		{
			(*it).second->Call(e);
		}
	}

	void EventHandler::SubscribeTo(EventHandler *handler, const TypeInfo &typeInfo, EventType type)
	{
		subscriptions.push_back(std::make_pair(handler, std::make_pair(typeInfo, type)));
	}
	void EventHandler::UnsubscribeTo(EventHandler *handler, const TypeInfo &typeInfo, EventType type)
	{
		for (SubscriptionList::iterator it = subscriptions.begin(); it != subscriptions.end(); ++it)
		{
			if ((*it).first == handler && (*it).second.first == typeInfo && (*it).second.second == type)
			{
				subscriptions.erase(it);
				break;
			}
		}
	}

	void EventHandler::Unsubscribe(EventType type, EventHandler *handler, const TypeInfo &typeInfo)
	{
		HandlerFunctionList &list = subscribers[typeInfo][type];
		for (HandlerFunctionList::iterator it = list.begin(); it != list.end(); ++it)
		{
			if ((*it).first == handler)
			{
				delete (*it).second;
				list.erase(it);
				handler->UnsubscribeTo(this, typeInfo, type);
				return;
			}
		}
	}

	bool EventHandler::HasHandler(HandlerFunctionList &list, EventHandler *handler) const
	{
		for (HandlerFunctionList::const_iterator it = list.cbegin(); it != list.cend(); ++it)
		{
			if ((*it).first == handler)
				return true;
		}
		return false;
	}
}
