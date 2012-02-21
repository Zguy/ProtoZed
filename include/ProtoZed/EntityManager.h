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
#ifndef PZ_EntityManager_h__
#define PZ_EntityManager_h__

#include <ProtoZed/Profiler.h>
#include <ProtoZed/HashString.h>
#include <ProtoZed/ObjectFactory/ObjectFactory.h>

#include <vector>
#include <unordered_map>

namespace PZ
{
	class MetaEntity;
	class Component;
	struct Message;

	typedef HashString EntityID;
	typedef std::vector<EntityID> EntityList;
	typedef std::unordered_map<EntityID, Component*, std::hash<int>> EntityComponentMap;
	typedef std::vector<HashString> ComponentList;

	class EntityListener
	{
	public:
		EntityListener()
		{}
		virtual ~EntityListener()
		{}

		virtual void EntityCreatedPre(const EntityID &id) {}
		virtual void EntityCreatedPost(const EntityID &id) {}
		
		virtual void EntityDestroyedPre(const EntityID &id) {}
		virtual void EntityDestroyedPost(const EntityID &id) {}

		virtual void EntitiesClearedPre() {}
		virtual void EntitiesClearedPost() {}

		virtual void ComponentAddedPre(const EntityID &id, const HashString &family) {}
		virtual void ComponentAddedPost(const EntityID &id, const HashString &family) {}
		
		virtual void ComponentRemovedPre(const EntityID &id, const HashString &family) {}
		virtual void ComponentRemovedPost(const EntityID &id, const HashString &family) {}
	};

	/**
	 * \brief	Manager for entities, components and the relationship between them.
	 */
	class EntityManager
	{
	public:
		EntityManager();
		~EntityManager();

		/**
		 * \brief	Creates an entity.
		 *
		 * \param	id	The identifier.
		 *
		 * \return	true if it succeeds, false if it fails.
		 */
		bool CreateEntity(const EntityID &id);

		/**
		 * \brief	Destroys the entity described by id.
		 *
		 * \param	id	The identifier.
		 *
		 * \return	true if it succeeds, false if it fails.
		 */
		bool DestroyEntity(const EntityID &id);

		bool HasEntity(const EntityID &id) const;

		/**
		 * \brief	Gets an entity.
		 *
		 * \param	id	The identifier.
		 *
		 * \return	The entity.
		 */
		MetaEntity GetEntity(const EntityID &id) const;

		void ClearEntities();

		/**
		 * \brief	Gets a list of every entity.
		 *
		 * \return	The list of entites.
		 */
		const EntityList &GetAllEntities() const;

		unsigned int GetEntityCount() const;

		/**
		 * \brief	Registers a component.
		 *
		 * \return	true if it succeeds, false if it fails.
		 */
		template<class T>
		bool RegisterComponent()
		{
			return factory.Register<T>(T::Family);
		}

		/**
		 * \brief	Unregisters a component.
		 *
		 * \return	true if it succeeds, false if it fails.
		 */
		template<class T>
		bool UnregisterComponent()
		{
			return factory.Unregister(T::Family);
		}

		/**
		 * \brief	Adds a component to 'id'.
		 *
		 * \param	id	The identifier.
		 *
		 * \return	null if it fails, else the component.
		 */
		template<class T>
		T *AddComponent(const EntityID &id)
		{
			T *component = dynamic_cast<T*>(factory.Create(T::Family, id, *this));

			if (component != nullptr)
			{
				if (AddComponentImpl(id, T::Family, component))
				{
					return component;
				}
				else
				{
					deleteComponent(component);
				}
			}

			return nullptr;
		}
		Component *AddComponent(const EntityID &id, const HashString &family)
		{
			Component *component = factory.Create(family, id, *this);

			if (component != nullptr)
			{
				if (AddComponentImpl(id, family, component))
				{
					return component;
				}
				else
				{
					deleteComponent(component);
				}
			}
			
			return nullptr;
		}

		/**
		 * \brief	Removes the component.
		 *
		 * \param	id	The identifier.
		 *
		 * \return	true if it succeeds, false if it fails.
		 */
		template<class T>
		bool RemoveComponent(const EntityID &id)
		{
			return RemoveComponentImpl(id, T::Family);
		}
		bool RemoveComponent(const EntityID &id, const HashString &family)
		{
			return RemoveComponentImpl(id, family);
		}

		/**
		 * \brief	Query if 'id' has a component.
		 *
		 * \param	id	The identifier.
		 *
		 * \return	true if it has the component, false if not.
		 */
		template<class T>
		bool HasComponent(const EntityID &id) const
		{
			return HasComponentImpl(id, T::Family);
		}
		bool HasComponent(const EntityID &id, const HashString &family) const
		{
			return HasComponentImpl(id, family);
		}

		/**
		 * \brief	Gets a component.
		 *
		 * \param	id	The identifier.
		 *
		 * \return	null if it fails, else the component.
		 */
		template<class T>
		T *GetComponent(const EntityID &id) const
		{
			return dynamic_cast<T*>(GetComponentImpl(id, T::Family));
		}
		Component *GetComponent(const EntityID &id, const HashString &family) const
		{
			return GetComponentImpl(id, family);
		}

		void GetAllComponents(const EntityID &id, ComponentList &list) const;

		/**
		 * \brief	Gets the entities with a certain component.
		 *
		 * \return	The entities with the component.
		 */
		template<class T>
		const EntityComponentMap &GetEntitiesWith() const
		{
			return GetEntitiesWithImpl(T::Family);
		}
		const EntityComponentMap &GetEntitiesWith(const HashString &family) const
		{
			return GetEntitiesWithImpl(family);
		}

		void SendMessageToAll(const Message &message) const;
		bool SendMessage(const Message &message, const EntityID &to) const;

		void RegisterListener(EntityListener *listener);
		void UnregisterListener(EntityListener *listener);

	private:
		bool AddComponentImpl(const EntityID &id, const HashString &name, Component *component);
		bool RemoveComponentImpl(const EntityID &id, const HashString &name);
		bool HasComponentImpl(const EntityID &id, const HashString &name) const;
		Component *GetComponentImpl(const EntityID &id, const HashString &name) const;
		const EntityComponentMap &GetEntitiesWithImpl(const HashString &name) const;

		static void deleteComponent(Component *component); // This is to avoid having to #include Component.h here

		typedef ObjectFactory<Component*(const EntityID&, EntityManager&), HashString> ComponentFactory;
		ComponentFactory factory;

		class Impl;
		Impl *p;
	};
}

#endif // PZ_EntityManager_h__