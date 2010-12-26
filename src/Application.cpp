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
#include "ProtoZed/Application.h"

#include <iostream>

#include <SFML/Graphics.hpp>

#include "ProtoZed/AppStateManager.h"

#include "ProtoZed/Entities/DrawableEntity.h"

#include "ProtoZed/Animation/AnimationGroup.h"
#include "ProtoZed/Animation/AttributeAnimation.h"

namespace PZ
{
	class ApplicationImpl
	{
	public:
		ApplicationImpl() : running(false)
		{
		}

		bool boot(const std::string &appName, sf::VideoMode &videoMode, sf::WindowSettings &params)
		{
			if (running)
				return true;

			window.Create(videoMode, appName, 6UL, params);

			entityManager.RegisterEntity<Entity>("Entity");
			entityManager.RegisterEntity<DrawableEntity>("DrawableEntity");

			animationManager.RegisterAnimationType<AnimationGroup>("AnimationGroup");
			animationManager.RegisterAnimationType<AttributeAnimation>("AttributeAnimation");

			running = true;

			return true;
		}

		void shutdown()
		{
			stateManager.PopAllStates();
			stateManager.Update();

			window.Close();

			running = false;
		}

		void handleInput() 
		{
			sf::Event event;
			while (window.GetEvent(event))
			{
				if (event.Type == sf::Event::Closed)
					running = false;
				else
				if (stateManager.GetCurrentState() != NULL)
				{
					if (event.Type == sf::Event::KeyPressed)
						stateManager.GetCurrentState()->OnKeyDown(event.Key);
					else if (event.Type == sf::Event::KeyReleased)
						stateManager.GetCurrentState()->OnKeyUp(event.Key);
					else if (event.Type == sf::Event::MouseButtonPressed)
						stateManager.GetCurrentState()->OnMouseDown(event.MouseButton);
					else if (event.Type == sf::Event::MouseButtonReleased)
						stateManager.GetCurrentState()->OnMouseUp(event.MouseButton);
					else if (event.Type == sf::Event::MouseMoved)
						stateManager.GetCurrentState()->OnMouseMove(event.MouseMove);
				}
			}
		}

		bool running;

		sf::RenderWindow window;

		AppStateManager  stateManager;
		EntityManager    entityManager;
		AnimationManager animationManager;

		ImageStorage       imageStorage;
		FontStorage        fontStorage;
		SoundBufferStorage soundBufferStorage;
	};

	Application::Application() : p(new ApplicationImpl)
	{
	}
	Application::~Application()
	{
		delete p;
	}

	int Application::Run(const std::string &appName, sf::VideoMode &videoMode, sf::WindowSettings &params)
	{
		if (!p->boot(appName, videoMode, params))
		{
			return 1;
		}

		while (p->running)
		{
			float deltaTime = p->window.GetFrameTime();

			p->stateManager.Update();

			p->handleInput();

			p->animationManager.Step(deltaTime);

			p->window.Clear();

			AppState *state = p->stateManager.GetCurrentState();
			if (state != NULL)
			{
				if (!state->Update(deltaTime))
				{
					p->stateManager.PopState();
				}

				// Draw drawable entities
				state->GetRootEntity()->HandleMessage(MessagePtr(new DrawMessage(p->window)));
			}

			p->window.Display();

			if ((!p->window.IsOpened())||
					(p->stateManager.IsEmpty()))
				p->running = false;
		}

		p->shutdown();

		return 0;
	}

	void Application::RequestShutdown()
	{
		p->running = false;
	}

	const sf::Input &Application::GetInput() const
	{
		return p->window.GetInput();
	}
	AppStateManager &Application::GetStateManager() const
	{
		return p->stateManager;
	}
	EntityManager &Application::GetEntityManager() const
	{
		return p->entityManager;
	}
	AnimationManager &Application::GetAnimationManager() const
	{
		return p->animationManager;
	}

	ImageStorage &Application::GetImageStorage() const
	{
		return p->imageStorage;
	}
	FontStorage  &Application::GetFontStorage() const
	{
		return p->fontStorage;
	}
	SoundBufferStorage &Application::GetSoundBufferStorage() const
	{
		return p->soundBufferStorage;
	}

	void Application::LogMessage(const std::string &message)
	{
		std::cout << message << std::endl;
	}
}