/*
	Copyright 2010 Johannes H�ggqvist

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
#include <ProtoZed/Entities/SoundEntity.h>

namespace PZ
{
	SoundEntity::SoundEntity(const std::string name) : Entity(name)
	{
	}
	SoundEntity::~SoundEntity()
	{
	}

	void SoundEntity::SetSoundBuffer(const sf::SoundBuffer &soundBuffer)
	{
		sound.SetBuffer(soundBuffer);
	}

	bool SoundEntity::OnMessage(MessagePtr message)
	{
		bool handled = Entity::OnMessage(message);

		if (message->message == MessageID::POSITION_UPDATED)
		{
			sf::Vector2f entityPos = GetGlobalPosition();
			sf::Vector3f soundPos(entityPos.x, entityPos.y, 0);
			sound.SetPosition(soundPos);

			return true;
		}

		return handled;
	}
}