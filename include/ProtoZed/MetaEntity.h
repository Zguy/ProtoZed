/*
Copyright (c) 2012 Johannes Häggqvist

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#ifndef PZ_MetaEntity_h__
#define PZ_MetaEntity_h__

#include <ProtoZed/EntityManager.h>

namespace PZ
{
	class Component;

	/**
	 * \brief	Meta entity. Used as a proxy to the EntityManager.
	 */
	class MetaEntity
	{
		friend class EntityManager;
	private:
		MetaEntity(const EntityID &id, EntityManager &manager) : id(id), manager(&manager)
		{}
	public:
		/**
		 * \brief Constructs an invalid object, to be used as a null object.
		 */
		MetaEntity() : id(EntityID()), manager(nullptr)
		{}
		~MetaEntity()
		{}

		/**
		 * \brief	Gets the identifier.
		 *
		 * \return	The identifier.
		 */
		inline const EntityID &GetID() const
		{
			return id;
		}

		/**
		 * \brief	Query if this object is valid.
		 *
		 * \return	true if valid, false if not.
		 */
		inline bool IsValid() const
		{
			return (id != EntityID() && manager != nullptr);
		}

		/**
		 * \brief	Destroys this entity.
		 *
		 * \return	true if it succeeds, false if it fails.
		 */
		bool Destroy()
		{
			if (IsValid())
				return manager->DestroyEntity(id);
			else
				return false;
		}

		/**
		 * \brief	Adds a component.
		 *
		 * \return	null if it fails, else the component.
		 */
		template<class T>
		T *AddComponent()
		{
			if (IsValid())
				return manager->AddComponent<T>(id);
			else
				return nullptr;
		}

		/**
		 * \brief	Removes a component.
		 *
		 * \return	true if it succeeds, false if it fails.
		 */
		template<class T>
		bool RemoveComponent()
		{
			if (IsValid())
				return manager->RemoveComponent<T>(id);
			else
				return false;
		}

		/**
		 * \brief	Query if the entity has component 'name'.
		 *
		 * \return	true if it has the component, false if not.
		 */
		template<class T>
		bool HasComponent() const
		{
			if (IsValid())
				return manager->HasComponent<T>(id);
			else
				return false;
		}

		/**
		 * \brief	Gets a component.
		 *
		 * \return	null if it fails, else the component.
		 */
		template<class T>
		T *GetComponent() const
		{
			if (IsValid())
				return manager->GetComponent<T>(id);
			else
				return nullptr;
		}

		bool SendMessage(const Message &message) const
		{
			if (IsValid())
				return manager->SendMessage(message, id);
			else
				return false;
		}

	private:
		EntityID id;

		EntityManager *manager;
	};
}

#if 0
#include <ProtoZed/Vector2.h>
#include <ProtoZed/Animation/Animable.h>
#include <ProtoZed/Attributes.h>
#include <ProtoZed/UniqueIDGenerator.h>
#include <ProtoZed/Messages.h>
#include <ProtoZed/ComponentFamily.h>
#include <ProtoZed/IncludeFilter.h>
#include <ProtoZed/IncludeFilters/NoFilter.h>

#include <string>
#include <vector>
#include <unordered_map>

namespace PZ
{
	class Entity;
	typedef std::vector<Entity*> EntityList;

	class Component;
	typedef std::unordered_map<std::string, Component*> ComponentMap;

	class Entity : public Animable
	{
	public:
		static Entity *Create(const std::string &entityName, const std::string &name = "");
		static void Destroy(Entity *entity);

		Entity(const std::string &name);
		virtual ~Entity();

		inline void Destroy() { Entity::Destroy(this); }

		inline bool HasParent() const { return (parent != nullptr); }
		inline Entity *GetParent() const { return parent; }
		const Entity *GetRoot() const;
		Entity *GetRoot();

		Entity *CreateChild(const std::string &entityName, const std::string &name = "");
		/**
		 * @brief Adds an Entity as a child
		 * 
		 * Will remove the Entity from the previous parent
		 * if it has one.
		 * 
		 * @param child The Entity to add
		 * @return True if successful
		 */
		bool AddChild(Entity *child);
		bool RemoveChild(Entity *child, bool destroy);
		void ClearChildren();

		inline const EntityList &GetChildren() const { return children; }
		void GetChildren(EntityList &list, const IncludeFilter &filter = NoFilter()) const;
		void GetChildrenRecursive(EntityList &list, const IncludeFilter &filter = NoFilter()) const;
		Entity *GetChildByIndex(unsigned int index) const;
		Entity *GetChildByName(const std::string &name, bool recursive = false) const;

		inline UniqueID GetID() const { return id; }
		inline const std::string &GetName() const { return name; }

		Component *CreateComponent(const std::string &name);
		template<class T>
		T *CreateComponent(const std::string &name)
		{
			return static_cast<T*>(CreateComponent(name));
		}
		bool AddComponent(Component *component);
		bool RemoveComponent(const std::string &name);
		void ClearComponents();

		Component *GetComponent(const std::string &name) const;
		template<class T>
		T *GetComponent(const std::string &name) const
		{
			return static_cast<T*>(GetComponent(name));
		}
		const ComponentMap &GetComponents() const { return components; }
		bool HasComponent(const std::string &name) const;

		const Vector2f &GetLocalPosition() const;
		const Vector2f GetGlobalPosition() const;

		void SetLocalPosition(const Vector2f &pos);
		void SetLocalPosition(float x, float y);
		void SetGlobalPosition(const Vector2f &pos);
		void SetGlobalPosition(float x, float y);

		inline void SetX(float x) { SetLocalPosition(x, GetLocalPosition().y); }
		inline void SetY(float y) { SetLocalPosition(GetLocalPosition().x, y); }

		inline void Move(float offsetX, float offsetY) { SetLocalPosition(GetLocalPosition().x+offsetX, GetLocalPosition().y+offsetY); }
		inline void Move(const Vector2f &offset) { SetLocalPosition(GetLocalPosition()+offset); }

		float GetLocalRotation() const;
		float GetGlobalRotation() const;

		void SetLocalRotation(float rot);
		void SetGlobalRotation(float rot);

		inline void Rotate(float angle) { SetLocalRotation(GetLocalRotation()+angle); }

		inline const Vector2f &GetLocalXAxis() const { return localXAxis; }
		inline const Vector2f &GetLocalYAxis() const { return localYAxis; }

		virtual bool HasAttribute(Attribute attribute) const;
		virtual void SetAttribute(Attribute attribute, float value);
		virtual float GetAttribute(Attribute attribute) const;

		bool BroadcastMessage(Message &message);
		bool ReceiveMessage(Message &message);
		bool SendMessage(Message &message, Entity *receiver);
		bool SendMessage(const Message &message, Entity *receiver);

		bool operator==(const Entity &other);

	protected:
		virtual bool HandleMessage(Message &message);

		void RecalculateLocalAxes();

	private:
		UniqueID id;

		Entity *parent;
		EntityList children;

		ComponentMap components;

		std::string name;
		Vector2f position;
		float rotation;
		Vector2f localXAxis;
		Vector2f localYAxis;
	};
}
#endif

#endif // PZ_MetaEntity_h__