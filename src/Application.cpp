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
#include <ProtoZed/Application.h>

#include <ProtoZed/Version.h>
#include <ProtoZed/Clock.h>
#include <ProtoZed/AppStateManager.h>
#include <ProtoZed/Profiler.h>
#include <ProtoZed/Log.h>
#include <ProtoZed/Messages.h>

#include <ProtoZed/Components/SceneNode.h>
#include <ProtoZed/Components/Position2D.h>
#include <ProtoZed/Components/Sprite.h>

#include <ProtoZed/Animation/AnimationGroup.h>
#include <ProtoZed/Animation/Tween.h>

namespace PZ
{
	class Application::Impl
	{
	public:
		Impl(Application &i) : i(i), running(false), services(i), stateManager(i)
		{
		}

		bool boot()
		{
			if (running)
				return true;

			profiler   = new Profiler;
			Profile profile("Boot");

			logManager = new LogManager;

			logManager->OpenLog("ProtoZed");
			logManager->GetLog("ProtoZed").Info(std::string("Initializing ProtoZed ")+Version::VERSION_STRING);

			// Start services
			services.StartAll();

			entityManager.RegisterComponent<SceneNode>();
			entityManager.RegisterComponent<Position2D>();
			entityManager.RegisterComponent<Sprite>();

			animationManager.RegisterAnimationType<AnimationGroup>("AnimationGroup");
			animationManager.RegisterAnimationType<Tween>("Tween");

			running = true;

			logManager->GetLog("ProtoZed").Info("ProtoZed is running");

			return true;
		}

		void shutdown()
		{
			{
				Profile profile("Shutdown");

				logManager->GetLog("ProtoZed").Info("Shutting down ProtoZed");

				stateManager.PopAllStates();
				stateManager.Update();

				// Shutdown services
				services.StopAll();
				services.RemoveAll();

				running = false;

				logManager->GetLog("ProtoZed").Info("ProtoZed has stopped");
			}

			delete logManager;

			profiler->WriteLog("Profile");
			delete profiler;
		}

		Application &i;

		bool running;

		Profiler   *profiler;
		LogManager *logManager;

		ServiceList      services;
		AppStateManager  stateManager;
		EntityManager    entityManager;
		AnimationManager animationManager;

		ImageStorage       imageStorage;
		FontStorage        fontStorage;
		SoundBufferStorage soundBufferStorage;
	};

	Application::Application()
	{
		p = new Impl(*this);
	}
	Application::~Application()
	{
		delete p;
	}

	int Application::Run()
	{
		if (!p->boot())
		{
			return 1;
		}

		Clock frameTimer;
		while (p->running)
		{
			Profile profile("MainLoop");

			float deltaTime = frameTimer.GetElapsedTime();
			frameTimer.Reset();

			{
				Profile profile("StateManager");
				p->stateManager.Update();
			}

			Update(deltaTime);

			{
				Profile profile("CurrentState");
				AppState *state = p->stateManager.GetCurrentState();
				if (state != nullptr)
				{
					state->Update(deltaTime);
				}
			}

			{
				Profile profile("UpdateComponents");
				p->entityManager.SendMessageToAll(UpdateMessage::Create(deltaTime));
			}

			p->services.UpdateAll(deltaTime);

			if (p->stateManager.IsEmpty())
			{
				RequestShutdown();
			}
		}

		p->shutdown();

		return 0;
	}

	void Application::RequestShutdown()
	{
		p->running = false;
	}

	ServiceList &Application::GetServiceList() const
	{
		return p->services;
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
}