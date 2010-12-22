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
#ifndef Entity_h__
#define Entity_h__

#include <string>
#include <vector>

#include <SFML/System/Vector2.hpp>

#define WEAK_SHARED_PTR(T) typedef std::shared_ptr<T> T ## Ptr; \
	                         typedef std::weak_ptr<T> T ## WeakPtr;

namespace PZ
{
	namespace MessageID
	{
		static const std::string POSITION_UPDATED = "PositionUpdated";
	}

	struct Message
	{
		Message(const std::string message = "", bool bubble = true, bool childrenFirst = false) : message(message), bubble(bubble), childrenFirst(childrenFirst)
		{}
		std::string message;
		bool bubble;
		bool childrenFirst; // Only applicable if bubble is true
	};
	typedef std::shared_ptr<Message> MessagePtr;

	class Entity;
	typedef std::vector<Entity*> EntityList;
	WEAK_SHARED_PTR(Entity)

	class Entity
	{
	public:
		Entity(const std::string &name, Entity *_parent = NULL);
		virtual ~Entity();

		void ChangeParent(Entity *newParent);
		inline bool HasParent() const { return (parent != NULL); }
		inline Entity *GetParent() const { return parent; }

		bool AddChild(EntityPtr child);
		bool AddChild(Entity *child);
		bool RemoveChild(EntityPtr child);
		bool RemoveChild(Entity *child);

		inline const EntityList &GetChildren() const { return children; }
		Entity *GetChildByIndex(unsigned int index) const;
		Entity *GetChildByName(const std::string name) const;

		inline const std::string &GetName() const { return name; }

		const sf::Vector2f &GetLocalPosition() const;
		const sf::Vector2f GetGlobalPosition() const;

		void SetLocalPosition(const sf::Vector2f &pos);
		void SetLocalPosition(float x, float y);
		void SetGlobalPosition(const sf::Vector2f &pos);
		void SetGlobalPosition(float x, float y);

		bool HandleMessage(MessagePtr message);

		bool operator==(const Entity &other);

	protected:
		virtual bool OnMessage(MessagePtr message) { return false; }

	private:
		Entity *parent;
		EntityList children;

		std::string name;
		sf::Vector2f position;
	};
}

#endif // Entity_h__