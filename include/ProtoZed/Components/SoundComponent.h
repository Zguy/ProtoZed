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
#ifndef SoundEntity_h__
#define SoundEntity_h__

#include <ProtoZed/Component.h>

#include <SFML/Audio/Sound.hpp>

namespace PZ
{
	class SoundComponent : public Component
	{
	public:
		SoundComponent(const EntityID &owner, EntityManager &manager);
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