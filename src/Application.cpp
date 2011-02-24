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
#include <ProtoZed/Application.h>

#include <ProtoZed/AppStateManager.h>

#include <ProtoZed/Entities/DrawableEntity.h>
#include <ProtoZed/Entities/SpritesheetEntity.h>
#include <ProtoZed/Entities/SoundEntity.h>

#include <ProtoZed/Animation/AnimationGroup.h>
#include <ProtoZed/Animation/Tween.h>

#include <SFML/Graphics.hpp>

namespace PZ
{
	class ApplicationImpl
	{
	public:
		ApplicationImpl() : running(false), listenerEntity(new ListenerEntity)
		{
		}

		bool boot(const std::string &appName, sf::VideoMode &videoMode, sf::WindowSettings &params)
		{
			if (running)
				return true;

			logManager.OpenLog("ProtoZed");
			logManager.GetLog("ProtoZed").Info(Log::LVL_LOW, "Initializing ProtoZed...");

			window.Create(videoMode, appName, 6UL, params);

			entityManager.RegisterEntity<Entity>("Entity");
			entityManager.RegisterEntity<DrawableEntity>("DrawableEntity");
			entityManager.RegisterEntity<SpritesheetEntity>("SpritesheetEntity");
			entityManager.RegisterEntity<SoundEntity>("SoundEntity");

			animationManager.RegisterAnimationType<AnimationGroup>("AnimationGroup");
			animationManager.RegisterAnimationType<Tween>("Tween");

			running = true;

			logManager.GetLog("ProtoZed").Info(Log::LVL_LOW, "ProtoZed is running");

			return true;
		}

		void shutdown()
		{
			logManager.GetLog("ProtoZed").Info(Log::LVL_LOW, "Shutting down ProtoZed...");

			stateManager.PopAllStates();
			stateManager.Update();

			window.Close();

			running = false;

			logManager.GetLog("ProtoZed").Info(Log::LVL_LOW, "ProtoZed has stopped");
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

		LogManager        logManager;
		AppStateManager   stateManager;
		EntityManager     entityManager;
		AnimationManager  animationManager;
		ListenerEntityPtr listenerEntity;

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

		// Stabilize framerate before entering main loop
		for (unsigned int i = 0; i < 10; ++i)
		{
			sf::Sleep(0.1f);
			p->window.Display();
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

				// Update entities
				state->GetRootEntity()->HandleMessage(MessagePtr(new UpdateMessage(deltaTime)));

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
	LogManager &Application::GetLogManager() const
	{
		return p->logManager;
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
	ListenerEntityPtr &Application::GetListenerEntity() const
	{
		return p->listenerEntity;
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
}