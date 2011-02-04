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

#include <ProtoZed/Entity.h>
#include <ProtoZed/ObjectFactory/ObjectFactory.h>
#include <ProtoZed/Convert.h>
#include <ProtoZed/UniqueIDGenerator.h>

namespace PZ
{
	typedef ::ObjectFactory<Entity*(const std::string&), std::string> EntityFactory;

	class EntityManager
	{
	public:
		EntityManager()
		{}
		~EntityManager()
		{}

		template<class T>
		bool RegisterEntity(const std::string &entityName)
		{
			return entityFactory.Register<T>(entityName);
		}
		bool UnregisterEntity(const std::string &entityName)
		{
			return entityFactory.Unregister(entityName);
		}

		EntityPtr GetNewEntity(const std::string &entityName, const std::string name = "")
		{
			std::string fixName = name;
			if (fixName.empty())
				fixName = "Entity"+Convert::ToString<UniqueID>(UniqueIDGenerator::GetNextID("Entity"));
			EntityPtr entity(entityFactory.Create(entityName, fixName));
			return entity;
		}
		template<class T>
		std::shared_ptr<T> GetNewEntity(const std::string &entityName, const std::string name = "")
		{
			EntityPtr entity = GetNewEntity(entityName, name);
			return std::static_pointer_cast<T, Entity>(entity);
		}

	private:
		EntityFactory entityFactory;
	};
}

#endif // EntityManager_h__