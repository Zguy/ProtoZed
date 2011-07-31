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
#include <ProtoZed/Components/SoundComponent.h>

namespace PZ
{
	SoundComponent::SoundComponent() : Component("Sound")
	{
	}
	SoundComponent::~SoundComponent()
	{
	}

	void SoundComponent::SetSoundBuffer(const sf::SoundBuffer &soundBuffer)
	{
		sound.SetBuffer(soundBuffer);
	}

	bool SoundComponent::HasAttribute(Attribute attribute) const
	{
		return ((attribute == Attributes::VOLUME)||
						Component::HasAttribute(attribute));
	}
	void SoundComponent::SetAttribute(Attribute attribute, float value)
	{
		Component::SetAttribute(attribute, value);

		if (attribute == Attributes::VOLUME)
			SetVolume(value);
	}
	float SoundComponent::GetAttribute(Attribute attribute) const
	{
		if (attribute == Attributes::VOLUME)
			return GetVolume();
		else
			return Component::GetAttribute(attribute);
	}

	bool SoundComponent::ReceiveMessage(Message &message)
	{
		if (message.message == MessageID::POSITION_UPDATED || message.message == MessageID::ROTATION_UPDATED)
		{
			sf::Vector2f entityPos = GetOwner()->GetGlobalPosition();
			sf::Vector3f soundPos(entityPos.x, entityPos.y, 0);
			sound.SetPosition(soundPos);

			return true;
		}

		return false;
	}
}