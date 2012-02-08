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
#include <ProtoZed/Entities/ListenerEntity.h>

#include <SFML/Audio/Listener.hpp>

namespace PZ
{
	ListenerEntity::ListenerEntity(const std::string &name) : Entity("ListenerEntity")
	{
	}
	ListenerEntity::~ListenerEntity()
	{
	}

	void ListenerEntity::SetGlobalVolume(float Volume)
	{
		sf::Listener::SetGlobalVolume(Volume);
	}
	float ListenerEntity::GetGlobalVolume() const
	{
		return sf::Listener::GetGlobalVolume();
	}

	bool ListenerEntity::HandleMessage(Message &message)
	{
		bool handled = Entity::HandleMessage(message);

		if (message.message == MessageID::POSITION_UPDATED || message.message == MessageID::ROTATION_UPDATED)
		{
			PZ::Vector2f entityPos = GetGlobalPosition();
			sf::Vector3f soundPos(entityPos.x, entityPos.y, 0);
			sf::Listener::SetPosition(soundPos);

			return true;
		}

		return handled;
	}
}