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
#ifndef SoundEntity_h__
#define SoundEntity_h__

#include <ProtoZed/Component.h>

#include <SFML/Audio/Sound.hpp>

namespace PZ
{
	class SoundComponent : public Component
	{
	public:
		SoundComponent();
		~SoundComponent();

		void SetSoundBuffer(const sf::SoundBuffer &soundBuffer);

		// Pretend we're a sf::Sound
		inline void Play() { sound.Play(); }
		inline void Pause() { sound.Pause(); }
		inline void Stop() { sound.Stop(); }

		inline void SetLoop(bool Loop) { sound.SetLoop(Loop); }
		inline void SetPitch(float Pitch) { sound.SetPitch(Pitch); }
		inline void SetVolume(float Volume) { sound.SetVolume(Volume); }

		inline void SetMinDistance(float MinDistance) { sound.SetMinDistance(MinDistance); }
		inline void SetAttenuation(float Attenuation) { sound.SetAttenuation(Attenuation); }

		inline void SetPlayingOffset(float TimeOffset) { sound.SetPlayingOffset(TimeOffset); }

		inline bool GetLoop() const { return sound.GetLoop(); }
		inline float GetPitch() const { return sound.GetPitch(); }
		inline float GetVolume() const { return sound.GetVolume(); }

		inline float GetMinDistance() const { return sound.GetMinDistance(); }
		inline float GetAttenuation() const { return sound.GetAttenuation(); }

		inline sf::Sound::Status GetStatus() const { return sound.GetStatus(); }

		inline float GetPlayingOffset() const { return sound.GetPlayingOffset(); }

		virtual bool HasAttribute(Attribute attribute) const;
		virtual void SetAttribute(Attribute attribute, float value);
		virtual float GetAttribute(Attribute attribute) const;

	protected:
		virtual bool HandleMessage(Message &message);

	private:
		sf::Sound sound;
	};
}

#endif // SoundEntity_h__