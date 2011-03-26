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

#include <SFML/System/Vector2.hpp>

#include <string>
#include <vector>

namespace PZ
{
	namespace MessageID
	{
		static const std::string UPDATE           = "Update";
		static const std::string POSITION_UPDATED = "PositionUpdated";
	}

	struct Message
	{
		enum Mode
		{
			STAY,  // Don't float or sink to any children
			FLOAT, // Go to the leaf child and float up to the surface
			SINK   // Sink down the leaf child
		};

		Message(const std::string &message = "", Mode mode = SINK) : message(message), mode(mode)
		{}
		std::string message;

		Mode mode;
	};

	struct UpdateMessage : public Message
	{
		UpdateMessage(float deltaTime) : Message(MessageID::UPDATE), deltaTime(deltaTime)
		{}

		float deltaTime;
	};

	class Entity;
	typedef std::vector<Entity*> EntityList;

	class Component;
	typedef std::vector<Component*> ComponentList;

	class Entity
	{
	public:
		Entity(const std::string &name);
		virtual ~Entity();

		inline bool HasParent() const { return (parent != NULL); }
		inline Entity *GetParent() const { return parent; }

		bool AddChild(Entity *child);
		bool RemoveChild(Entity *child);

		inline const EntityList &GetChildren() const { return children; }
		Entity *GetChildByIndex(unsigned int index) const;
		Entity *GetChildByName(const std::string name, bool recursive = false) const;

		inline const std::string &GetName() const { return name; }

		void AddComponent(Component *component);
		void RemoveComponent(const std::string &name);
		Component *GetComponent(const std::string &name) const;

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

		bool HandleMessage(Message &message);

		bool operator==(const Entity &other);

	protected:
		void RecalculateLocalAxes();

		virtual bool OnMessage(Message &message);

	private:
		UniqueID id;

		Entity *parent;
		EntityList children;

		ComponentList components;

		std::string name;
		sf::Vector2f position;
		float rotation;
		sf::Vector2f localXAxis;
		sf::Vector2f localYAxis;
	};
}

#endif // Entity_h__