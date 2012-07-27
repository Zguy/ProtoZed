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

#include <ProtoZed/NonCopyable.h>
#include <ProtoZed/EventHandler.h>
#include <ProtoZed/Profiler.h>
#include <ProtoZed/HashString.h>
#include <ProtoZed/ObjectFactory/ObjectFactory.h>

#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace PZ
{
	class Application;
	class MetaEntity;
	class Component;
	class Archetype;

	typedef HashString EntityID;
	typedef std::vector<EntityID> EntityList;
	typedef std::vector<HashString> ComponentList;
	typedef std::unordered_set<EntityID, std::hash<int>> EntitySet;
	typedef std::unordered_map<EntityID, Component*, std::hash<int>> EntityComponentMap;

	class EntityEvent : public Event
	{
	public:
		enum State { CREATED, DESTROYED };
		EntityEvent(State state, const EntityID &id, bool post = true) : state(state), id(id), post(post) {};
		State state;
		EntityID id;
		bool post;
	};
	class EntitiesClearedEvent : public Event
	{
	public:
		EntitiesClearedEvent(bool post = true) : post(post) {};
		bool post;
	};
	class ComponentEvent : public Event
	{
	public:
		enum State { ADDED, REMOVED };
		ComponentEvent(State state, const EntityID &id, const HashString &family, bool post = true) : state(state), id(id), family(family), post(post) {};
		State state;
		EntityID id;
		HashString family;
		bool post;
	};

	/**
	 * \brief	Manager for entities, components and the relationship between them.
	 */
	class EntityManager : public NonCopyable, public EventHandler
	{
	public:
		EntityManager(Application &application);
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
		 * \brief	Creates an entity.
		 *
		 * \param	id						The identifier.
		 * \param [out]	entity	The created entity (will be unchanged if it fails).
		 *
		 * \return	true if it succeeds, false if it fails.
		 */
		bool CreateEntity(const EntityID &id, MetaEntity &entity);

		/**
		 * \brief	Creates an entity from an archetype.
		 *
		 * \param	name	The name of the archetype.
		 * \param	id		The identifier.
		 *
		 * \return	true if it succeeds, false if it fails.
		 */
		bool CreateFromArchetype(const std::string &name, const EntityID &id);

		/**
		 * \brief	Creates an entity from an archetype.
		 *
		 * \param	name					The name of the archetype.
		 * \param	id						The identifier.
		 * \param [out]	entity	The created entity (will be unchanged if it fails).
		 *
		 * \return	true if it succeeds, false if it fails.
		 */
		bool CreateFromArchetype(const std::string &name, const EntityID &id, MetaEntity &entity);

		/**
		 * \brief	Flags the entity for destruction.
		 *
		 * \param	id	The identifier.
		 *
		 * \return	true if it succeeds, false if it fails.
		 */
		bool DestroyEntity(const EntityID &id);

		/**
		 * \brief	Destroys flagged entities.
		 */
		void DestroyPendingEntities();

		/**
		 * \brief	Query if the entity 'id' exists.
		 *
		 * \param	id	The identifier.
		 *
		 * \return	true if it has the entity, false if not.
		 */
		bool HasEntity(const EntityID &id) const;

		/**
		 * \brief	Gets an entity.
		 *
		 * \param	id	The identifier.
		 *
		 * \return	The entity.
		 */
		MetaEntity GetEntity(const EntityID &id) const;

		/**
		 * \brief	Clears all entities.
		 */
		void ClearEntities();

		/**
		 * \brief	Gets a list of every entity.
		 *
		 * \return	The list of entites.
		 */
		void GetAllEntities(EntityList &list) const;

		/**
		 * \brief	Gets the number of entities.
		 *
		 * \return	The entity count.
		 */
		EntityList::size_type GetEntityCount() const;

		/**
		 * \brief	Registers an archetype.
		 *
		 * EntityManager takes ownership of the Archetype instance.
		 *
		 * \param [in]	archetype The archetype.
		 *
		 * \return	true if it succeeds, false if the archetype is already registered.
		 */
		bool RegisterArchetype(Archetype *archetype);

		/**
		 * \brief	Unregisters the archetype described by name.
		 *
		 * \param	name	The name.
		 *
		 * \return	true if it succeeds, false if the archetype wasn't found.
		 */
		bool UnregisterArchetype(const std::string &name);

		/**
		 * \brief	Unregisters all archetypes.
		 */
		void UnregisterAllArchetypes();

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
		 * \brief	Gets all registered components.
		 *
		 * \param [in,out]	list	The list.
		 */
		void GetAllRegisteredComponents(std::vector<HashString> &list) const;

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
			T *component = dynamic_cast<T*>(factory.Create(T::Family));

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

		/**
		 * \brief	Adds a component to 'id'.
		 *
		 * \param	id			The identifier.
		 * \param	family	The family of the component.
		 *
		 * \return	null if it fails, else the component.
		 */
		Component *AddComponent(const EntityID &id, const HashString &family)
		{
			Component *component = factory.Create(family);

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

		/**
		 * \brief	Removes the component.
		 *
		 * \param	id			The identifier.
		 * \param	family	The family of the component.
		 *
		 * \return	true if it succeeds, false if it fails.
		 */
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

		/**
		 * \brief	Query if 'id' has a component.
		 *
		 * \param	id			The identifier.
		 * \param	family	The family of the component.
		 *
		 * \return	true if it has the component, false if not.
		 */
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

		/**
		 * \brief	Gets a component.
		 *
		 * \param	id			The identifier.
		 * \param	family	The family of the component.
		 *
		 * \return	null if it fails, else the component.
		 */
		Component *GetComponent(const EntityID &id, const HashString &family) const
		{
			return GetComponentImpl(id, family);
		}

		/**
		 * \brief	Gets all components for an entity.
		 *
		 * \param	id							The identifier.
		 * \param [in,out]	list	The list.
		 */
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

		/**
		 * \brief	Gets all components for an entity.
		 *
		 * \param	family	The family of the component.
		 *
		 * \return	The entities with the component.
		 */
		const EntityComponentMap &GetEntitiesWith(const HashString &family) const
		{
			return GetEntitiesWithImpl(family);
		}

		/**
		 * \brief	Updates all components.
		 *
		 * \param	deltaTime	Time since last frame.
		 */
		void UpdateAll(float deltaTime);

	private:
		bool AddComponentImpl(const EntityID &id, const HashString &name, Component *component);
		bool RemoveComponentImpl(const EntityID &id, const HashString &name);
		bool HasComponentImpl(const EntityID &id, const HashString &name) const;
		Component *GetComponentImpl(const EntityID &id, const HashString &name) const;
		const EntityComponentMap &GetEntitiesWithImpl(const HashString &name) const;

		static void deleteComponent(Component *component); // This is to avoid having to #include Component.h here

		typedef ObjectFactory<Component*(), HashString> ComponentFactory;
		ComponentFactory factory;

		class Impl;
		Impl *p;
	};
}

#endif // PZ_EntityManager_h__
