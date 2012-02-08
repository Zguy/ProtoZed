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
#include <ProtoZed/Components/SoundComponent.h>

namespace PZ
{
	SoundComponent::SoundComponent(const EntityID &owner, EntityManager &manager) : Component(owner, manager)
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

	bool SoundComponent::HandleMessage(Message &message)
	{
		if (message.message == MessageID::POSITION_UPDATED || message.message == MessageID::ROTATION_UPDATED)
		{
			PZ::Vector2f entityPos = GetOwner()->GetGlobalPosition();
			sf::Vector3f soundPos(entityPos.x, entityPos.y, 0);
			sound.SetPosition(soundPos);

			return true;
		}

		return false;
	}
}