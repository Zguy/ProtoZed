/*
	Copyright 2010-2011 Johannes H�ggqvist

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

#include <ProtoZed/Version.h>
#include <ProtoZed/AppStateManager.h>

#include <ProtoZed/Entities/ListenerEntity.h>

#include <ProtoZed/Components/DrawableComponent.h>
#include <ProtoZed/Components/SpritesheetComponent.h>
#include <ProtoZed/Components/SoundComponent.h>

#include <ProtoZed/Animation/AnimationGroup.h>
#include <ProtoZed/Animation/Tween.h>

#include <SFML/Graphics.hpp>

namespace PZ
{
	class ApplicationImpl
	{
	public:
		ApplicationImpl() : running(false)
		{
		}

		bool boot(const std::string &appName, VideoMode &videoMode, unsigned long windowStyle, const sf::WindowSettings &params)
		{
			if (running)
				return true;

			logManager.OpenLog("ProtoZed");
			logManager.GetLog("ProtoZed").Info(std::string("Initializing ProtoZed ")+Version::VERSION_STRING);

			sf::VideoMode sfVideoMode(videoMode.GetWindowResolution().x, videoMode.GetWindowResolution().y);
			if (videoMode.GetFullscreen())
				windowStyle |= sf::Style::Fullscreen;
			window.Create(sfVideoMode, appName, windowStyle, params);

			view = videoMode.GetView();
			window.SetView(view);

			entityManager.RegisterEntity<Entity>("Entity");
			entityManager.RegisterEntity<ListenerEntity>("ListenerEntity");

			componentManager.RegisterComponent<DrawableComponent>("Drawable");
			componentManager.RegisterComponent<SpritesheetComponent>("Spritesheet");
			componentManager.RegisterComponent<SoundComponent>("Sound");

			animationManager.RegisterAnimationType<AnimationGroup>("AnimationGroup");
			animationManager.RegisterAnimationType<Tween>("Tween");

			running = true;

			logManager.GetLog("ProtoZed").Info("ProtoZed is running");

			return true;
		}

		void shutdown()
		{
			logManager.GetLog("ProtoZed").Info("Shutting down ProtoZed");

			stateManager.PopAllStates();
			stateManager.Update();

			window.Close();

			running = false;

			logManager.GetLog("ProtoZed").Info("ProtoZed has stopped");
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
					if (event.Type == sf::Event::TextEntered)
						stateManager.GetCurrentState()->OnTextInput(event.Text);
					else if (event.Type == sf::Event::KeyPressed)
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
		sf::View view;
		Input input;

		LogManager       logManager;
		AppStateManager  stateManager;
		EntityManager    entityManager;
		ComponentManager componentManager;
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

	int Application::Run(const std::string &appName, VideoMode &videoMode, unsigned long windowStyle, const sf::WindowSettings &params)
	{
		if (!p->boot(appName, videoMode, windowStyle, params))
		{
			return 1;
		}

		while (p->running)
		{
			float deltaTime = p->window.GetFrameTime();

			p->input.Update(p->window);

			p->stateManager.Update();

			p->handleInput();

			p->window.Clear();

			Update(deltaTime);

			AppState *state = p->stateManager.GetCurrentState();
			if (state != NULL)
			{
				if (!state->Update(deltaTime))
				{
					p->stateManager.PopState();
				}

				// Update entities
				state->GetRootEntity()->ReceiveMessage(UpdateMessage(deltaTime));

				// Draw drawable entities
				state->GetRootEntity()->ReceiveMessage(DrawMessage(p->window));
			}

			p->window.Display();

			if ((!p->window.IsOpened())||(p->stateManager.IsEmpty()))
				p->running = false;
		}

		p->shutdown();

		return 0;
	}

	void Application::RequestShutdown()
	{
		p->running = false;
	}

	const Input &Application::GetInput() const
	{
		return p->input;
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
	ComponentManager &Application::GetComponentManager() const
	{
		return p->componentManager;
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
}