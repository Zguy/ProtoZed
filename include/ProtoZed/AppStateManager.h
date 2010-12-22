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
#ifndef AppStateManager_h__
#define AppStateManager_h__

#include <vector>
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>

#include "AppState.h"
#include "ObjectFactory/ObjectFactory.h"

namespace PZ
{
	typedef ::ObjectFactory<AppState*(), std::string> AppStateFactory;

	class AppStateManagerImpl;

	class AppStateManager
	{
	public:
		AppStateManager();
		~AppStateManager();

		void Update();

		template<class T>
		bool RegisterState(const std::string &stateName)
		{
			return getAppStateFactory().Register<T>(stateName);
		}
		bool UnregisterState(const std::string &stateName);

		void ChangeState(const std::string &stateName, StringMap *const options = NULL);
		void PushState(const std::string &stateName, StringMap *const options = NULL);
		void PopState();
		void PopAllStack();

		bool IsEmpty();

		AppState *GetCurrentState() const;

	private:
		AppStateManagerImpl *p;

		AppStateFactory &getAppStateFactory() const;
	};
}

#endif // AppStateManager_h__