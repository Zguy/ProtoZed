/*
Copyright (c) 2013 Johannes Häggqvist

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

#include <ProtoZed/SystemList.h>
#include <ProtoZed/StateStack.h>
#include <ProtoZed/EntityManager.h>
#include <ProtoZed/Animation/AnimationManager.h>
#include <ProtoZed/AssetLibrary.h>

#include <ProtoZed/Components/SceneNode.h>
#include <ProtoZed/Components/Position2D.h>
#include <ProtoZed/Components/Sprite.h>
#include <ProtoZed/Components/SoundEmitter.h>
#include <ProtoZed/Components/Spritesheet.h>

#include <ProtoZed/Animation/AnimationGroup.h>
#include <ProtoZed/Animation/Tween.h>

namespace PZ
{
	class Application::Impl
	{
	public:
		Impl(Application &i) : i(i), running(false), systems(i), stateStack(i), entityManager(i), assetLibrary(i)
		{
		}

		bool boot()
		{
			if (running)
				return true;

			if (LogManager::GetSingletonPtr() == nullptr)
				new LogManager;
			if (Profiler::GetSingletonPtr() == nullptr)
				new Profiler;

			LogManager::GetSingleton().Open("ProtoZed");
			Log::Info("ProtoZed", std::string("Initializing ProtoZed ")+Version::VERSION_STRING);

			entityManager.RegisterComponent<SceneNode>();
			entityManager.RegisterComponent<Position2D>();
			entityManager.RegisterComponent<Sprite>();
			entityManager.RegisterComponent<SoundEmitter>();
			entityManager.RegisterComponent<Spritesheet>();

			i.SetTimescale(1.f);

			i.Start();

			// Start systems
			systems.StartAll();

			running = true;

			Log::Info("ProtoZed", "ProtoZed is running");

			return true;
		}

		void shutdown()
		{
			Log::Info("ProtoZed", "Shutting down ProtoZed");

			stateStack.PopAll();
			stateStack.Update();

			entityManager.ClearEntities();
			entityManager.DestroyPendingEntities();

			assetLibrary.UnloadAll();

			// Shutdown systems
			systems.StopAll();
			systems.RemoveAll();

			i.Stop();

			running = false;

			Log::Info("ProtoZed", "ProtoZed has stopped");

			Profiler::GetSingleton().WriteLog("Profile");

			delete LogManager::GetSingletonPtr();
			delete Profiler::GetSingletonPtr();
		}

		Application &i;

		bool running;

		float timescale;

		SystemList       systems;
		StateStack       stateStack;
		EntityManager    entityManager;
		AnimationManager animationManager;
		AssetLibrary     assetLibrary;
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

		Profiler::GetSingleton().Start();

		Clock frameTimer;
		while (p->running)
		{
			float deltaTimeNonScaled = frameTimer.GetElapsedTime();
			float deltaTime = deltaTimeNonScaled * p->timescale;
			frameTimer.Reset();

			{
				Profile profile("StateStack");
				p->stateStack.Update();
			}

			p->assetLibrary.RunGroupQueue();

			Update(deltaTime);

			p->systems.UpdateAll(deltaTime);

			{
				Profile profile("CurrentState");
				State *state = p->stateStack.GetCurrent();
				if (state != nullptr)
				{
					state->Update(deltaTime);
				}
			}

			p->entityManager.UpdateAll(deltaTime);

			p->entityManager.DestroyPendingEntities();

			if (p->stateStack.IsEmpty())
			{
				RequestShutdown();
			}

			Profiler::GetSingleton().NextFrame();
		}

		Profiler::GetSingleton().Stop();

		p->shutdown();

		return 0;
	}

	void Application::RequestShutdown()
	{
		p->running = false;
	}

	float Application::GetTimescale() const
	{
		return p->timescale;
	}
	void Application::SetTimescale(float timescale)
	{
		p->timescale = timescale;
	}

	SystemList &Application::GetSystemList() const
	{
		return p->systems;
	}
	StateStack &Application::GetStateStack() const
	{
		return p->stateStack;
	}
	EntityManager &Application::GetEntityManager() const
	{
		return p->entityManager;
	}
	AnimationManager &Application::GetAnimationManager() const
	{
		return p->animationManager;
	}
	AssetLibrary &Application::GetAssetLibrary() const
	{
		return p->assetLibrary;
	}
}