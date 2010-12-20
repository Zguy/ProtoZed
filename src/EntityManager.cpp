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
#include "ProtoZed/EntityManager.h"

namespace PZ
{
	class EntityManagerImpl
	{
	public:
		Entity *rootEntity;

		EntityFactory entityFactory;
	};

	EntityManager::EntityManager()
	{
		p = new EntityManagerImpl;

		p->rootEntity = new Entity("RootEntity", NULL);
	}
	EntityManager::~EntityManager()
	{
		delete p->rootEntity;

		delete p;
	}

	bool EntityManager::UnregisterEntity(const std::string &entityName)
	{
		return p->entityFactory.Unregister(entityName);
	}

	Entity *EntityManager::GetNewEntity(const std::string &entityName, const std::string name, Entity *parent)
	{
		return p->entityFactory.Create(entityName, name, parent);
	}

	Entity *const EntityManager::GetRootEntity()
	{
		return p->rootEntity;
	}

	EntityFactory &EntityManager::getEntityFactory()
	{
		return p->entityFactory;
	}
}