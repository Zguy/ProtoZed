/*
	Copyright 2010-2011 Johannes Häggqvist

	This file is part of ProtoZed.

	ProtoZed is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	ProtoZed is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with ProtoZed.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <ProtoZed/Entities/ListenerEntity.h>

#include <SFML/Audio/Listener.hpp>

namespace PZ
{
	ListenerEntity::ListenerEntity() : Entity("ListenerEntity")
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

	bool ListenerEntity::OnMessage(Message &message)
	{
		bool handled = Entity::OnMessage(message);

		if (message.message == MessageID::POSITION_UPDATED)
		{
			sf::Vector2f entityPos = GetGlobalPosition();
			sf::Vector3f soundPos(entityPos.x, entityPos.y, 0);
			sf::Listener::SetPosition(soundPos);

			return true;
		}

		return handled;
	}
}