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
#include <ProtoZed/Entities/SoundEntity.h>

namespace PZ
{
	SoundEntity::SoundEntity(const std::string &name) : Entity(name, "Sound")
	{
	}
	SoundEntity::~SoundEntity()
	{
	}

	void SoundEntity::SetSoundBuffer(const sf::SoundBuffer &soundBuffer)
	{
		sound.SetBuffer(soundBuffer);
	}

	bool SoundEntity::HasAttribute(Attribute attribute) const
	{
		return ((attribute == Attributes::VOLUME)||
						Entity::HasAttribute(attribute));
	}
	void SoundEntity::SetAttribute(Attribute attribute, float value)
	{
		Entity::SetAttribute(attribute, value);

		if (attribute == Attributes::VOLUME)
			SetVolume(value);
	}
	float SoundEntity::GetAttribute(Attribute attribute) const
	{
		if (attribute == Attributes::VOLUME)
			return GetVolume();
		else
			return Entity::GetAttribute(attribute);
	}

	bool SoundEntity::HandleMessage(Message &message)
	{
		bool handled = Entity::HandleMessage(message);

		if (message.message == MessageID::POSITION_UPDATED || message.message == MessageID::ROTATION_UPDATED)
		{
			sf::Vector2f entityPos = GetGlobalPosition();
			sf::Vector3f soundPos(entityPos.x, entityPos.y, 0);
			sound.SetPosition(soundPos);

			return true;
		}

		return handled;
	}
}