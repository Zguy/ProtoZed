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
	EntityManager::EntityManager()
	{
	}
	EntityManager::~EntityManager()
	{
	}

	bool EntityManager::UnregisterEntity(const std::string &entityName)
	{
		return entityFactory.Unregister(entityName);
	}

	Entity *EntityManager::GetNewEntity(const std::string &entityName, const std::string name, Entity *parent)
	{
		return entityFactory.Create(entityName, name, parent);
	}

	void EntityManager::DeleteEntity(Entity *entity)
	{
		delete entity;
	}
}