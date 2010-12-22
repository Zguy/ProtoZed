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
#ifndef AppState_h__
#define AppState_h__

#include <map>
#include <string>

#include <SFML/Window/Event.hpp>

//#include "Application.h"
#include "Entity.h"

namespace PZ
{
	typedef std::map<std::string, std::string> StringMap;

	class AppState
	{
	public:
		AppState() : started(false)//, rootEntity(Application::GetSingleton().GetEntityManager().GetNewEntity("Entity", "RootEntity", NULL))
		{
			rootEntity = new Entity("RootEntity");
		}
		virtual ~AppState()
		{
			delete rootEntity;
		}

		virtual bool Update(float deltaTime) = 0;

		virtual void Start(StringMap *const options) { started = true; }
		virtual void Stop() { started = false; }

		virtual void Pause() {}
		virtual void Resume() {}

		virtual void OnKeyDown(const sf::Event::KeyEvent &keyEvent) {}
		virtual void OnKeyUp(const sf::Event::KeyEvent &keyEvent) {}
		virtual void OnMouseDown(const sf::Event::MouseButtonEvent &mouseButtonEvent) {}
		virtual void OnMouseUp(const sf::Event::MouseButtonEvent &mouseButtonEvent) {}
		virtual void OnMouseMove(const sf::Event::MouseMoveEvent &mouseMoveEvent) {}

		virtual void LoadAssets() {}
		virtual void UnloadAssets() {}

		inline Entity *const GetRootEntity() const { return rootEntity; }

		inline bool IsStarted() const { return started; }

	protected:
		bool started;

	private:
		Entity *rootEntity;
	};
}

#endif // AppState_h__