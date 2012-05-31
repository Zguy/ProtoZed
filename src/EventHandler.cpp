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

#include <map>
#include <vector>
#include <algorithm>

namespace PZ
{
	class EventHandler::Impl
	{
	public:
		typedef std::vector<EventHandler*> EventHandlerList;
		typedef std::map<EventType, EventHandlerList> EventSubscriberMap;
		EventSubscriberMap eventSubscribers;
	};

	EventHandler::EventHandler() : p(new Impl)
	{
	}
	EventHandler::~EventHandler()
	{
		delete p;
	}

	bool EventHandler::Subscribe(EventHandler &handler, const EventType &eventType)
	{
		Impl::EventSubscriberMap::iterator it = p->eventSubscribers.find(eventType);
		if (it != p->eventSubscribers.end())
		{
			(*it).second.push_back(&handler);
			return true;
		}

		return false;
	}
	bool EventHandler::Unsubscribe(EventHandler &handler, const EventType &eventType)
	{
		Impl::EventSubscriberMap::iterator it = p->eventSubscribers.find(eventType);
		if (it != p->eventSubscribers.end())
		{
			Impl::EventHandlerList::const_iterator it2 = std::find((*it).second.cbegin(), (*it).second.cend(), &handler);
			if (it2 != (*it).second.cend())
			{
				(*it).second.erase(it2);
				return true;
			}
		}

		return false;
	}

	void EventHandler::EmitEvent(Event &e) const
	{
		Impl::EventSubscriberMap::iterator it = p->eventSubscribers.find(e.GetType());
		if (it != p->eventSubscribers.end())
		{
			Impl::EventHandlerList &list = (*it).second;
			for (Impl::EventHandlerList::iterator it = list.begin(); it != list.end(); ++it)
			{
				(*it)->HandleEvent(e);
			}
		}
	}
}
