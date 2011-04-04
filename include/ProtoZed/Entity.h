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
#ifndef Entity_h__
#define Entity_h__

#include <ProtoZed/UniqueIDGenerator.h>
#include <ProtoZed/Messages.h>

#include <SFML/System/Vector2.hpp>

#include <string>
#include <vector>

namespace PZ
{
	class Entity;
	typedef std::vector<Entity*> EntityList;

	class Component;
	typedef std::vector<Component*> ComponentList;

	class Entity
	{
	public:
		Entity(const std::string &name);
		Entity(const std::string &name, const std::string &family);
		virtual ~Entity();

		inline bool HasParent() const { return (parent != NULL); }
		inline Entity *GetParent() const { return parent; }
		const Entity *GetRoot() const;
		Entity *GetRoot();

		bool AddChild(Entity *child);
		bool RemoveChild(Entity *child);

		inline const EntityList &GetChildren() const { return children; }
		void GetChildrenRecursive(EntityList &list) const;
		Entity *GetChildByIndex(unsigned int index) const;
		Entity *GetChildByName(const std::string name, bool recursive = false) const;

		inline UniqueID GetID() const { return id; }
		inline const std::string &GetName() const { return name; }
		inline const std::string &GetFamily() const { return family; }

		bool AddComponent(Component *component);
		bool AddComponent(const std::string &name);
		
		bool RemoveComponent(const std::string &name, bool destroy = true);
		void ClearComponents(bool destroy = true);

		Component *GetComponent(const std::string &name) const;
		template<class T>
		T *GetComponent(const std::string &name) const
		{
			return static_cast<T*>(GetComponent(name));
		}

		bool HasComponent(const std::string &name) const;

		const sf::Vector2f &GetLocalPosition() const;
		const sf::Vector2f GetGlobalPosition() const;

		void SetLocalPosition(const sf::Vector2f &pos);
		void SetLocalPosition(float x, float y);
		void SetGlobalPosition(const sf::Vector2f &pos);
		void SetGlobalPosition(float x, float y);

		inline void SetX(float x) { SetLocalPosition(x, GetLocalPosition().y); }
		inline void SetY(float y) { SetLocalPosition(GetLocalPosition().x, y); }

		inline void Move(float offsetX, float offsetY) { SetLocalPosition(GetLocalPosition().x+offsetX, GetLocalPosition().y+offsetY); }
		inline void Move(const sf::Vector2f& offset) { SetLocalPosition(GetLocalPosition()+offset); }

		float GetLocalRotation() const;
		float GetGlobalRotation() const;

		void SetLocalRotation(float rot);
		void SetGlobalRotation(float rot);

		inline void Rotate(float angle) { SetLocalRotation(GetLocalRotation()+angle); }

		inline const sf::Vector2f &GetLocalXAxis() const { return localXAxis; }
		inline const sf::Vector2f &GetLocalYAxis() const { return localYAxis; }

		bool BroadcastMessage(Message &message);
		bool ReceiveMessage(Message &message);

		bool operator==(const Entity &other);

	protected:
		virtual bool HandleMessage(Message &message);

		void RecalculateLocalAxes();

	private:
		UniqueID id;

		Entity *parent;
		EntityList children;

		ComponentList components;

		std::string name;
		std::string family;
		sf::Vector2f position;
		float rotation;
		sf::Vector2f localXAxis;
		sf::Vector2f localYAxis;
	};
}

#endif // Entity_h__