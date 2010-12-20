/*
	Copyright 2010 Johannes Häggqvist

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

#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include "Singleton.h"
#include "AppStateManager.h"
#include "EntityManager.h"
#include "AssetStorage.h"
#include "Animation/AnimationManager.h"

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

		int Run(const std::string &appName, sf::VideoMode &videoMode, sf::WindowSettings &params = sf::WindowSettings());

		void RequestShutdown();

		const sf::Input    &GetInput();
		AppStateManager    &GetStateManager();
		EntityManager      &GetEntityManager();
		AnimationManager   &GetAnimationManager();
		ImageStorage       &GetImageStorage();
		FontStorage        &GetFontStorage();
		SoundBufferStorage &GetSoundBufferStorage();

		void LogMessage(const std::string &message);

	private:
		ApplicationImpl *p;
	};
}

#endif // Application_h__