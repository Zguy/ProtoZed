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

			animationManager.RegisterAnimationType<AnimationGroup>("AnimationGroup");
			animationManager.RegisterAnimationType<AttributeAnimation>("AttributeAnimation");

			running = true;

			return true;
		}

		void shutdown()
		{
			stateManager.PopAllStack();
			stateManager.Update();
			//stateManager.DeleteAllStates();

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

		AppStateManager stateManager;

		AnimationManager   animationManager;
		ImageManager       imageManager;
		FontManager        fontManager;
		SoundBufferManager soundBufferManager;
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

				state->Draw(p->window);
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

	/*AppState *Application::GetStateByName(const std::string &stateName)
	{
		return p->stateManager.GetStateByName(stateName);
	}
	void Application::AddState(const std::string &stateName, AppState *state)
	{
		p->stateManager.AddState(stateName, state);
	}
	void Application::RemoveState(const std::string &stateName)
	{
		p->stateManager.RemoveState(stateName);
	}
	void Application::ChangeState(const std::string &stateName, StringMap *const options)
	{
		p->stateManager.ChangeState(stateName, options);
	}
	void Application::PushState(const std::string &stateName, StringMap *const options)
	{
		p->stateManager.PushState(stateName, options);
	}
	void Application::PopState()
	{
		p->stateManager.PopState();
	}*/

	AppStateManager &Application::GetStateManager()
	{
		return p->stateManager;
	}
	sf::RenderWindow &Application::GetRenderWindow()
	{
		return p->window;
	}
	const sf::Input &Application::GetInput()
	{
		return p->window.GetInput();
	}
	AnimationManager &Application::GetAnimationManager()
	{
		return p->animationManager;
	}
	ImageManager &Application::GetImageManager()
	{
		return p->imageManager;
	}
	FontManager  &Application::GetFontManager()
	{
		return p->fontManager;
	}
	SoundBufferManager &Application::GetSoundBufferManager()
	{
		return p->soundBufferManager;
	}

	void Application::LogMessage(const std::string &message)
	{
		std::cout << message << std::endl;
	}
}