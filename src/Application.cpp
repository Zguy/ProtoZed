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
#include <ProtoZed/Profiler.h>
#include <ProtoZed/Log.h>
#include <ProtoZed/Messages.h>

#include <ProtoZed/ServiceList.h>
#include <ProtoZed/AppStateManager.h>
#include <ProtoZed/EntityManager.h>
#include <ProtoZed/Animation/AnimationManager.h>
#include <ProtoZed/AssetManager.h>
#include <ProtoZed/Random.h>

#include <ProtoZed/Components/SceneNode.h>
#include <ProtoZed/Components/Position2D.h>
#include <ProtoZed/Components/Sprite.h>
#include <ProtoZed/Components/SoundEmitter.h>

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

			Profile profile("Boot");

			logManager.Open("ProtoZed");
			Log::Info("ProtoZed", std::string("Initializing ProtoZed ")+Version::VERSION_STRING);

			entityManager.RegisterComponent<SceneNode>();
			entityManager.RegisterComponent<Position2D>();
			entityManager.RegisterComponent<Sprite>();
			entityManager.RegisterComponent<SoundEmitter>();

			i.Start();

			// Start services
			services.StartAll();

			running = true;

			Log::Info("ProtoZed", "ProtoZed is running");

			return true;
		}

		void shutdown()
		{
			{
				Profile profile("Shutdown");

				Log::Info("ProtoZed", "Shutting down ProtoZed");

				stateManager.PopAll();
				stateManager.Update();

				i.Stop();

				entityManager.ClearEntities();

				assetManager.UnloadAll();

				// Shutdown services
				services.StopAll();
				services.RemoveAll();

				running = false;

				Log::Info("ProtoZed", "ProtoZed has stopped");
			}

			profiler.WriteLog("Profile");
		}

		Application &i;

		bool running;

		ServiceList      services;
		AppStateManager  stateManager;
		EntityManager    entityManager;
		AnimationManager animationManager;
		AssetManager     assetManager;
		RandomManager    randomManager;
		LogManager       logManager;
		Profiler         profiler;
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

			Update(deltaTime);

			p->services.UpdateAll(deltaTime);

			{
				Profile profile("StateManager");
				p->stateManager.Update();
			}

			{
				Profile profile("CurrentState");
				AppState *state = p->stateManager.GetCurrent();
				if (state != nullptr)
				{
					state->Update(deltaTime);
				}
			}

			{
				Profile profile("UpdateComponents");
				p->entityManager.UpdateAll(deltaTime);
			}

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
	AssetManager &Application::GetAssetManager() const
	{
		return p->assetManager;
	}
	RandomManager &Application::GetRandomManager() const
	{
		return p->randomManager;
	}
}