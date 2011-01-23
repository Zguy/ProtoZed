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
#include <ProtoZed/Entity.h>

namespace PZ
{
	Entity::Entity(const std::string &name) : parent(NULL), name(name), position(0.f,0.f)
	{
		id = UniqueIDGenerator::GetNextID("Entity");
	}
	Entity::~Entity()
	{
		if (HasParent())
		{
			parent->RemoveChild(this);
		}

		for (EntityList::iterator it = children.begin(); it != children.end(); ++it)
		{
			(*it)->parent = NULL;
		}
		children.clear();
	}

	bool Entity::AddChild(EntityPtr child)
	{
		if (*child == *this)
			return false;

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

		return !found;
	}
	bool Entity::RemoveChild(EntityPtr child)
	{
		return RemoveChild(child.get());
	}
	bool Entity::RemoveChild(Entity *child)
	{
		bool found = false;
		for (EntityList::iterator it = children.begin(); it != children.end(); ++it)
		{
			if (*(*it) == *child)
			{
				(*it)->parent = NULL;
				children.erase(it);
				found = true;
				break;
			}
		}

		return found;
	}

	EntityPtr Entity::GetChildByIndex(unsigned int index) const
	{
		if (index < children.size())
		{
			return children[index];
		}
		else
		{
			return NULL;
		}
	}
	EntityPtr Entity::GetChildByName(const std::string name) const
	{
		for (EntityList::const_iterator it = children.cbegin(); it != children.cend(); ++it)
		{
			if ((*it)->GetName() == name)
				return (*it);
		}
		return NULL;
	}

	const sf::Vector2f &Entity::GetLocalPosition() const
	{
		return position;
	}
	const sf::Vector2f Entity::GetGlobalPosition() const
	{
		if (HasParent())
			return parent->GetGlobalPosition() + position;
		else
			return position;
	}

	void Entity::SetLocalPosition(const sf::Vector2f &pos)
	{
		position = pos;

		HandleMessage(MessagePtr(new Message(MessageID::POSITION_UPDATED)));
	}
	void Entity::SetLocalPosition(float x, float y)
	{
		position.x = x;
		position.y = y;

		HandleMessage(MessagePtr(new Message(MessageID::POSITION_UPDATED)));
	}
	void Entity::SetGlobalPosition(const sf::Vector2f &pos)
	{
		if (HasParent())
			position = pos - parent->GetGlobalPosition();
		else
			position = pos;

		HandleMessage(MessagePtr(new Message(MessageID::POSITION_UPDATED)));
	}
	void Entity::SetGlobalPosition(float x, float y)
	{
		if (HasParent())
		{
			sf::Vector2f parentPos = parent->GetGlobalPosition();
			position.x = x - parentPos.x;
			position.y = y - parentPos.y;
		}
		else
		{
			position.x = x;
			position.y = y;
		}

		HandleMessage(MessagePtr(new Message(MessageID::POSITION_UPDATED)));
	}

	bool Entity::HandleMessage(MessagePtr message)
	{
		if (message->mode == Message::FLOAT)
		{
			for (EntityList::iterator it = children.begin(); it != children.end(); ++it)
			{
				(*it)->HandleMessage(message);
			}
		}

		bool handled = OnMessage(message);

		if (message->mode == Message::SINK)
		{
			for (EntityList::iterator it = children.begin(); it != children.end(); ++it)
			{
				(*it)->HandleMessage(message);
			}
		}

		return handled;
	}

	bool Entity::operator==(const Entity &other)
	{
		return id == other.id;
	}
}