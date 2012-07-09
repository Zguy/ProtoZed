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
		while (!subscribers.empty())
		{
			EventHandler *handler = subscribers.front();
			handler->UnsubscribeTo(this);
		}

		while (!subscriptions.empty())
		{
			EventHandler *handler = subscriptions.front();
			UnsubscribeTo(handler);
		}

		for (HandlerMap::iterator it = handlers.begin(); it != handlers.end(); ++it)
		{
			delete (*it).second;
			(*it).second = nullptr;
		}
		handlers.clear();
	}

	bool EventHandler::SubscribeTo(EventHandler *handler)
	{
		if (handler->Subscribe(this))
		{
			subscriptions.push_back(handler);
			return true;
		}

		return false;
	}
	bool EventHandler::UnsubscribeTo(EventHandler *handler)
	{
		if (handler->Unsubscribe(this))
		{
			subscriptions.erase(std::find(subscriptions.begin(), subscriptions.end(), handler));
			return true;
		}

		return false;
	}

	void EventHandler::HandleEvent(const Event &e)
	{
		HandlerMap::iterator it = handlers.find(TypeInfo(typeid(e)));
		if (it != handlers.end())
		{
			(*it).second->Exec(e);
		}
	}

	void EventHandler::EmitEvent(const Event &e) const
	{
		for (SubscriberList::const_iterator it = subscribers.cbegin(); it != subscribers.cend(); ++it)
		{
			(*it)->HandleEvent(e);
		}
	}

	bool EventHandler::Subscribe(EventHandler *handler)
	{
		SubscriberList::iterator it = std::find(subscribers.begin(), subscribers.end(), handler);
		if (it == subscribers.end())
		{
			subscribers.push_back(handler);
			return true;
		}

		return false;
	}
	bool EventHandler::Unsubscribe(EventHandler *handler)
	{
		SubscriberList::iterator it = std::find(subscribers.begin(), subscribers.end(), handler);
		if (it != subscribers.end())
		{
			subscribers.erase(it);
			return true;
		}

		return false;
	}
}
