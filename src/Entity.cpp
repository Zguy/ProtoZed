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
#include <ProtoZed/Entity.h>

#include <ProtoZed/Convert.h>
#include <ProtoZed/Application.h>
#include <ProtoZed/Component.h>

namespace PZ
{
	Entity *Entity::Create(const std::string &entityName, const std::string &name)
	{
		return PZ::Application::GetSingleton().GetEntityManager().CreateEntity(entityName, name);
	}
	void Entity::Destroy(Entity *entity, bool destroyChildren)
	{
		PZ::Application::GetSingleton().GetEntityManager().DestroyEntity(entity, destroyChildren);
	}

	Entity::Entity(const std::string &name) : parent(NULL), name(name), family("Entity"), position(0.f,0.f), rotation(0.f), localXAxis(1.f,0.f), localYAxis(0.f,1.f)
	{
		id = UniqueIDGenerator::GetNextID("EntityID");
	}
	Entity::Entity(const std::string &name, const std::string &family) : parent(NULL), name(name), family(family), position(0.f,0.f), rotation(0.f), localXAxis(1.f,0.f), localYAxis(0.f,1.f)
	{
		id = UniqueIDGenerator::GetNextID("EntityID");
	}
	Entity::~Entity()
	{
		if (HasParent())
		{
			parent->RemoveChild(this, false);
		}

		ClearChildren(false);

		ClearComponents(true);
	}

	const Entity *Entity::GetRoot() const
	{
		const Entity *entity = this;
		while (entity->HasParent())
		{
			entity = entity->GetParent();
		}
		return entity;
	}
	Entity *Entity::GetRoot()
	{
		Entity *entity = this;
		while (entity->HasParent())
		{
			entity = entity->GetParent();
		}
		return entity;
	}

	Entity *Entity::CreateChild(const std::string &entityName, const std::string &name)
	{
		Entity *entity = PZ::Application::GetSingleton().GetEntityManager().CreateEntity(entityName, name);
		if (AddChild(entity))
		{
			return entity;
		}
		else
		{
			PZ::Application::GetSingleton().GetEntityManager().DestroyEntity(entity);
			return NULL;
		}
	}
	bool Entity::AddChild(Entity *child)
	{
		if (child == NULL)
		{
			Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Warning(Log::LVL_LOW, "AddChild() ignored a NULL pointer");
			return false;
		}

		if (*child == *this)
		{
			Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Error(Log::LVL_MEDIUM, "Entity \""+GetName()+"\" tried to add itself as a child");
			return false;
		}

		bool found = false;
		for (EntityList::iterator it = children.begin(); it != children.end(); ++it)
		{
			if ((*(*it) == *child)||((*it)->GetName() == child->GetName()))
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			sf::Vector2f oldPos = child->GetGlobalPosition();
			if (child->HasParent())
				child->GetParent()->RemoveChild(child);
			child->parent = this;
			child->SetGlobalPosition(oldPos);
			children.push_back(child);
		}
		else
		{
			Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Error(Log::LVL_MEDIUM, "Entity \""+GetName()+"\" already has child \""+child->GetName()+"\"");
		}

		return !found;
	}
	bool Entity::RemoveChild(Entity *child, bool destroy)
	{
		bool found = false;
		for (EntityList::iterator it = children.begin(); it != children.end(); ++it)
		{
			if (*(*it) == *child)
			{
				(*it)->parent = NULL;

				if (destroy)
					Application::GetSingleton().GetEntityManager().DestroyEntity(*it, true);

				children.erase(it);

				found = true;
				break;
			}
		}

		if (!found)
		{
			Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Error(Log::LVL_MEDIUM, "Entity \""+GetName()+"\" tried to remove non-existent child \""+child->GetName()+"\"");
		}

		return found;
	}
	void Entity::ClearChildren(bool destroy)
	{
		for (EntityList::iterator it = children.begin(); it != children.end(); ++it)
		{
			(*it)->parent = NULL;

			if (destroy)
				Application::GetSingleton().GetEntityManager().DestroyEntity(*it, true);
		}
		children.clear();
	}

	void Entity::GetChildrenRecursive(EntityList &list) const
	{
		for (EntityList::const_iterator it = children.cbegin(); it != children.cend(); ++it)
		{
			list.push_back(*it);
			(*it)->GetChildrenRecursive(list);
		}
	}
	Entity *Entity::GetChildByIndex(unsigned int index) const
	{
		if (index < children.size())
		{
			return children[index];
		}
		else
		{
			Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Error(Log::LVL_MEDIUM, "Entity \""+GetName()+"\" tried to access non-existent child with index \""+Convert::ToString<unsigned int>(index)+"\"");
			return NULL;
		}
	}
	Entity *Entity::GetChildByName(const std::string &name, bool recursive) const
	{
		const Entity *const parent = this;
		for (EntityList::const_iterator it = parent->children.cbegin(); it != parent->children.cend(); ++it)
		{
			if ((*it)->GetName() == name)
				return (*it);

			if (recursive)
			{
				Entity *child = (*it)->GetChildByName(name, true);
				if (child != NULL)
					return child;
			}
		}
		return NULL;
	}

	Component *Entity::CreateComponent(const std::string &name)
	{
		Component *component = Application::GetSingleton().GetComponentManager().CreateComponent(name);
		if (AddComponent(component))
		{
			return component;
		}
		else
		{
			Application::GetSingleton().GetComponentManager().DestroyComponent(component);
			return NULL;
		}
	}
	bool Entity::AddComponent(Component *component)
	{
		if (component == NULL)
		{
			Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Warning(Log::LVL_LOW, "AddComponent() ignored a NULL pointer");
			return false;
		}

		if (component->GetOwner() == NULL && !HasComponent(component->GetName()))
		{
			component->SetOwner(this);
			components.insert(std::make_pair(component->GetName(), component));
			return true;
		}
		else
		{
			Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Error(Log::LVL_MEDIUM, "Entity \""+GetName()+"\" tried to add component \""+component->GetName()+"\", which already has an owner.");
			return false;
		}
	}
	bool Entity::RemoveComponent(const std::string &name, bool destroy)
	{
		ComponentList::const_iterator it = components.find(name);
		if (it != components.end())
		{
			Component *component = (*it).second;
			component->SetOwner(NULL);

			if (destroy)
			{
				Application::GetSingleton().GetComponentManager().DestroyComponent(component);
			}

			components.erase(it);

			return true;
		}
		else
		{
			Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Error(Log::LVL_MEDIUM, "Entity \""+GetName()+"\" tried to remove non-existent component \""+name+"\"");
			return false;
		}
	}
	void Entity::ClearComponents(bool destroy)
	{
		for (ComponentList::iterator it = components.begin(); it != components.end(); ++it)
		{
			Component *component = (*it).second;

			component->SetOwner(NULL);

			if (destroy)
			{
				Application::GetSingleton().GetComponentManager().DestroyComponent(component);
			}
		}
		components.clear();
	}

	Component *Entity::GetComponent(const std::string &name) const
	{
		ComponentList::const_iterator it = components.find(name);
		if (it != components.cend())
		{
			return (*it).second;
		}
		else
		{
			Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Error(Log::LVL_MEDIUM, "Entity \""+GetName()+"\" tried to access non-existent component \""+name+"\"");
			return NULL;
		}
	}
	bool Entity::HasComponent(const std::string &name) const
	{
		return (components.find(name) != components.cend());
	}

	const sf::Vector2f &Entity::GetLocalPosition() const
	{
		return position;
	}
	const sf::Vector2f Entity::GetGlobalPosition() const
	{
		if (HasParent())
		{
			const sf::Vector2f &parentXAxis = parent->GetLocalXAxis();
			const sf::Vector2f &parentYAxis = parent->GetLocalYAxis();

			const sf::Vector2f rotatedPos = position.x * parentXAxis + position.y * parentYAxis;

			return parent->GetGlobalPosition() + rotatedPos;
		}
		else
		{
			return position;
		}
	}

	void Entity::SetLocalPosition(const sf::Vector2f &pos)
	{
		position = pos;

		ReceiveMessage(Message(MessageID::POSITION_UPDATED));
	}
	void Entity::SetLocalPosition(float x, float y)
	{
		position.x = x;
		position.y = y;

		ReceiveMessage(Message(MessageID::POSITION_UPDATED));
	}
	void Entity::SetGlobalPosition(const sf::Vector2f &pos)
	{
		if (HasParent())
		{
			const float angle = -Convert::DegreesToRadians(-parent->GetGlobalRotation());
			sf::Vector2f xAxis, yAxis;
			xAxis.x = std::cos(angle);
			xAxis.y = std::sin(angle);
			yAxis.x = -xAxis.y;
			yAxis.y = xAxis.x;

			const sf::Vector2f aaPos = pos - parent->GetGlobalPosition();
			const sf::Vector2f rotatedPos = aaPos.x * xAxis + aaPos.y * yAxis;

			position = rotatedPos;
		}
		else
		{
			position = pos;
		}

		ReceiveMessage(Message(MessageID::POSITION_UPDATED));
	}
	void Entity::SetGlobalPosition(float x, float y)
	{
		if (HasParent())
		{
			SetGlobalPosition(sf::Vector2f(x,y));
		}
		else
		{
			position.x = x;
			position.y = y;
		}

		ReceiveMessage(Message(MessageID::POSITION_UPDATED));
	}

	float Entity::GetLocalRotation() const
	{
		return rotation;
	}
	float Entity::GetGlobalRotation() const
	{
		if (HasParent())
			return parent->GetGlobalRotation() + rotation;
		else
			return rotation;
	}

	void Entity::SetLocalRotation(float rot)
	{
		rotation = rot;

		ReceiveMessage(Message(MessageID::ROTATION_UPDATED));
	}
	void Entity::SetGlobalRotation(float rot)
	{
		if (HasParent())
			rotation = rot - parent->GetGlobalRotation();
		else
			rotation = rot;

		ReceiveMessage(Message(MessageID::ROTATION_UPDATED));
	}

	bool Entity::HasAttribute(Attribute attribute) const
	{
		bool has = false;

		for (ComponentList::const_iterator it = components.cbegin(); it != components.cend(); ++it)
		{
			Component *component = (*it).second;
			if (component->HasAttribute(attribute))
			{
				has = true;
				break; // It doesn't matter if more components has the attribute, we just need "has" to be true
			}
		}

		if (!has) // Pretty much the same reason has above
		{
			if ((attribute == Attributes::X)||
					(attribute == Attributes::Y)||
					(attribute == Attributes::ROTATION))
			{
				has = true;
			}
		}

		return has;
	}
	void Entity::SetAttribute(Attribute attribute, float value)
	{
		for (ComponentList::iterator it = components.begin(); it != components.end(); ++it)
		{
			Component *component = (*it).second;
			if (component->HasAttribute(attribute))
			{
				component->SetAttribute(attribute, value);
			}
		}

		if (attribute == Attributes::X)
			SetX(value);
		else if (attribute == Attributes::Y)
			SetY(value);
		else if (attribute == Attributes::ROTATION)
			SetLocalRotation(value);
	}
	float Entity::GetAttribute(Attribute attribute) const
	{
		// There's no way to choose which to return if more than one component has the attribute,
		// so we'll just return the first we find and hope that its value is representable for the whole Entity
		float result = 0.f;

		for (ComponentList::const_iterator it = components.cbegin(); it != components.cend(); ++it)
		{
			Component *component = (*it).second;
			if (component->HasAttribute(attribute))
			{
				result = component->GetAttribute(attribute);
				break;
			}
		}

		if (attribute == Attributes::X)
			result = GetLocalPosition().x;
		else if (attribute == Attributes::Y)
			result = GetLocalPosition().y;
		else if (attribute == Attributes::ROTATION)
			result = GetLocalRotation();

		return result;
	}

	bool Entity::BroadcastMessage(Message &message)
	{
		return GetRoot()->ReceiveMessage(message);
	}
	bool Entity::ReceiveMessage(Message &message)
	{
		if (message.mode == Message::FLOAT)
		{
			for (EntityList::iterator it = children.begin(); it != children.end(); ++it)
			{
				(*it)->ReceiveMessage(message);
			}
		}

		bool handled = HandleMessage(message);
		for (ComponentList::iterator it = components.begin(); it != components.end(); ++it)
		{
			Component *component = (*it).second;
			if (component->ReceiveMessage(message))
				handled = true;
		}

		if (message.mode == Message::SINK)
		{
			for (EntityList::iterator it = children.begin(); it != children.end(); ++it)
			{
				(*it)->ReceiveMessage(message);
			}
		}

		return handled;
	}

	bool Entity::operator==(const Entity &other)
	{
		return id == other.id;
	}

	bool Entity::HandleMessage(Message &message)
	{
		if (message.message == MessageID::ROTATION_UPDATED)
		{
			RecalculateLocalAxes();

			return true;
		}
		else if (message.message == MessageID::UPDATE)
		{
			UpdateMessage &updateMessage = message.As<UpdateMessage>();
			float deltaTime = updateMessage.deltaTime;

			StepAnimations(deltaTime);

			return true;
		}

		return false;
	}

	void Entity::RecalculateLocalAxes()
	{
		const float angle = -Convert::DegreesToRadians(GetGlobalRotation());
		localXAxis.x = std::cos(angle);
		localXAxis.y = std::sin(angle);
		localYAxis.x = -localXAxis.y;
		localYAxis.y = localXAxis.x;
	}
}