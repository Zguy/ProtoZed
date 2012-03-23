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
#include <Sound_SFML.h>

#include <sfSoundAsset.h>

#include <ProtoZed/Components/SoundEmitter.h>
#include <ProtoZed/Components/Position2D.h>

#include <ProtoZed/Application.h>
#include <ProtoZed/AssetManager.h>

#include <SFML/Audio.hpp>

namespace PZ
{
	typedef std::map<PZ::EntityID, std::pair<PZ::Timestamp, sf::Sound>> SoundMap;

	class Sound_SFML::Impl
	{
	public:
		Impl(Sound_SFML *i) : i(i)
		{}

		SoundMap::iterator findSound(const EntityID &id)
		{
			return sounds.find(id);
		}

		void updateSound(sf::Sound &sound, const SoundEmitter *emitter)
		{
			AssetManager &assetManager = i->GetApplication().GetAssetManager();

			if (!emitter->GetFile().empty())
			{
				const sfSoundAsset *soundAsset = assetManager.Get<sfSoundAsset>(emitter->GetFile());
				if (soundAsset != nullptr)
				{
					sound.SetBuffer(soundAsset->GetSound());
				}
			}

			sound.SetLoop(emitter->IsRepeating());
			sound.SetVolume(emitter->GetVolume());
		}
		SoundMap::iterator createSound(const EntityID &id, const SoundEmitter *emitter)
		{
			sf::Sound sound;

			updateSound(sound, emitter);

			return sounds.insert(std::make_pair(id, std::make_pair(emitter->GetTimestamp(), sound))).first;
		}
		void deleteLayer(const PZ::EntityID &id)
		{
			SoundMap::iterator it = findSound(id);
			if (it != sounds.end())
			{
				sounds.erase(it);
			}
		}
		void clearLayers()
		{
			sounds.clear();
		}

		Sound_SFML *i;

		SoundMap sounds;
	};

	Sound_SFML::Sound_SFML(const ServiceType &type, Application &application) : Sound(type, application)
	{
		p = new Impl(this);
	}
	Sound_SFML::~Sound_SFML()
	{
		delete p;
	}

	bool Sound_SFML::Start()
	{
		if (!Sound::Start())
			return false;

		return true;
	}
	bool Sound_SFML::Stop()
	{
		if (!Sound::Stop())
			return false;

		return true;
	}

	void Sound_SFML::Update(float deltaTime)
	{
		const EntityComponentMap &ecm = GetApplication().GetEntityManager().GetEntitiesWith<SoundEmitter>();
		for (EntityComponentMap::const_iterator it = ecm.cbegin(); it != ecm.cend(); ++it)
		{
			const EntityID &id = (*it).first;
			const SoundEmitter *emitter = static_cast<SoundEmitter*>((*it).second);

			SoundMap::iterator soundIt = p->findSound(id);
			if (soundIt == p->sounds.end())
			{
				soundIt = p->createSound(id, emitter);
			}

			sf::Sound &sound = (*soundIt).second.second;

			Timestamp &timestamp = (*soundIt).second.first;
			if (emitter->GetTimestamp() != timestamp)
			{
				p->updateSound(sound, emitter);
				timestamp = emitter->GetTimestamp();
			}

			if ((sound.GetStatus() == sf::Sound::Playing) && !emitter->IsPlaying())
			{
				sound.Stop();
			}
			else if ((sound.GetStatus() == sf::Sound::Stopped) && emitter->IsPlaying())
			{
				sound.Play();
			}

			Position2D *position = GetApplication().GetEntityManager().GetComponent<Position2D>(id);
			if (position != nullptr)
			{
				const Vector2f &pos = position->GetPosition(Scope::GLOBAL);
				sound.SetPosition(sf::Vector3f(pos.x, pos.y, 0.f));
			}
		}
	}
}
