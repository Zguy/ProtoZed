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
#include <ProtoZed/EntityManager.h>

#include <ProtoZed/Convert.h>
#include <ProtoZed/UniqueIDGenerator.h>
#include <ProtoZed/Application.h>

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

	Entity *EntityManager::CreateEntity(const std::string &entityName, const std::string &name)
	{
		std::string fixName = name;
		if (fixName.empty())
			fixName = "Entity"+Convert::ToString<UniqueID>(UniqueIDGenerator::GetNextID("EntityName"));
		Entity *entity = entityFactory.Create(entityName, fixName);
		if (entity != NULL)
			Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Info("Created entity \""+entity->GetName()+"\" ("+Convert::ToString<UniqueID>(entity->GetID())+")");
		else
			Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Error("Tried to create entity \""+entityName+"\", but it does not exist");
		return entity;
	}

	void EntityManager::DestroyEntity(Entity *entity) const
	{
		if (entity == NULL)
		{
			Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Warning("DestroyEntity() ignored a NULL pointer");
			return;
		}

		// Destroy the children
		EntityList children = entity->GetChildren();
		for (EntityList::iterator it = children.begin(); it != children.end(); ++it)
		{
			Entity *child = (*it);
			DestroyEntity(child);
		}

		Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Info("Destroyed entity \""+entity->GetName()+"\" ("+Convert::ToString<UniqueID>(entity->GetID())+")");

		delete entity;
	}
}