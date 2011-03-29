/*
	Copyright 2010-2011 Johannes Häggqvist

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
#ifndef ComponentManager_h__
#define ComponentManager_h__

#include <ProtoZed/Component.h>
#include <ProtoZed/ObjectFactory/ObjectFactory.h>

namespace PZ
{
	class ComponentManager
	{
	public:
		ComponentManager();
		~ComponentManager();

		template<class T>
		bool RegisterComponent(const std::string &componentName)
		{
			return componentFactory.Register<T>(componentName);
		}
		bool UnregisterComponent(const std::string &componentName);

		Component *GetNewComponent(const std::string &componentName);
		template<class T>
		T *GetNewComponent(const std::string &componentName)
		{
			Component *component = GetNewComponent(componentName);
			return static_cast<T*>(component);
		}

		void DestroyComponent(Component *component) const;

	private:
		typedef ::ObjectFactory<Component*(), std::string> ComponentFactory;
		ComponentFactory componentFactory;
	};
}

#endif // ComponentManager_h__