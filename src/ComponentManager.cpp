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
#include <ProtoZed/ComponentManager.h>

#include <ProtoZed/Application.h>

namespace PZ
{
	ComponentManager::ComponentManager()
	{
	}
	ComponentManager::~ComponentManager()
	{
	}

	bool ComponentManager::UnregisterComponent(const std::string &entityName)
	{
		return componentFactory.Unregister(entityName);
	}

	Component *ComponentManager::CreateComponent(const std::string &componentName)
	{
		Component *component = componentFactory.Create(componentName);
		if (component != NULL)
			Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Info(Log::LVL_LOW, "Created component \""+component->GetName()+"\"");
		else
			Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Error(Log::LVL_MEDIUM, "Tried to create component \""+componentName+"\", but it does not exist");
		return component;
	}

	void ComponentManager::DestroyComponent(Component *component) const
	{
		if (component == NULL)
		{
			Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Warning(Log::LVL_LOW, "DestroyComponent() ignored a NULL pointer");
			return;
		}

		Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Info(Log::LVL_LOW, "Destroyed component \""+component->GetName()+"\"");
		delete component;
	}
}