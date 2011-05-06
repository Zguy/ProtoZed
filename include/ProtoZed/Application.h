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
#ifndef Application_h__
#define Application_h__

#include <ProtoZed/Singleton.h>
#include <ProtoZed/VideoMode.h>
#include <ProtoZed/Input.h>
#include <ProtoZed/AppStateManager.h>
#include <ProtoZed/EntityManager.h>
#include <ProtoZed/ComponentManager.h>
#include <ProtoZed/Logging/LogManager.h>
#include <ProtoZed/Animation/AnimationManager.h>
#include <ProtoZed/AssetStorage.h>

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include <string>

namespace PZ
{
	typedef AssetStorage<sf::Image>       ImageStorage;
	typedef AssetStorage<sf::Font>        FontStorage;
	typedef AssetStorage<sf::SoundBuffer> SoundBufferStorage;

	class ApplicationImpl;

	class Application : public Singleton<Application>
	{
	public:
		Application();
		~Application();

		virtual void Update(float deltaTime) {}

		int Run(const std::string &appName, VideoMode &videoMode, unsigned long windowStyle = (sf::Style::Close | sf::Style::Resize), sf::WindowSettings &params = sf::WindowSettings());

		void RequestShutdown();

		const Input        &GetInput() const;
		LogManager         &GetLogManager() const;
		AppStateManager    &GetStateManager() const;
		EntityManager      &GetEntityManager() const;
		ComponentManager   &GetComponentManager() const;
		AnimationManager   &GetAnimationManager() const;

		ImageStorage       &GetImageStorage() const;
		FontStorage        &GetFontStorage() const;
		SoundBufferStorage &GetSoundBufferStorage() const;

	private:
		ApplicationImpl *p;
	};
}

#endif // Application_h__