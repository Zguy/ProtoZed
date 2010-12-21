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
#ifndef EntityManager_h__
#define EntityManager_h__

#include "Entity.h"
#include "ObjectFactory/ObjectFactory.h"

namespace PZ
{
	typedef ::ObjectFactory<Entity*(const std::string&, Entity*), std::string> EntityFactory;

	class EntityManager
	{
	public:
		EntityManager();
		~EntityManager();

		template<class T>
		bool RegisterEntity(const std::string &entityName)
		{
			return entityFactory.Register<T>(entityName);
		}
		bool UnregisterEntity(const std::string &entityName);

		Entity *GetNewEntity(const std::string &entityName, const std::string name, Entity *parent = NULL);
		template<class T>
		T *GetNewEntity(const std::string &entityName, const std::string name, Entity *parent = NULL)
		{
			return static_cast<T*>(GetNewEntity(entityName, name, parent));
		}

		void DeleteEntity(Entity *entity);

	private:
		EntityFactory entityFactory;
	};
}

#endif // EntityManager_h__