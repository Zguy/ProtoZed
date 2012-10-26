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
		 * \brief	Query if 'id' is about to be destroyed.
		 *
		 * \param	id	The identifier.
		 *
		 * \return	true if alive, false if not.
		 */
		bool IsAlive(const EntityID &id) const;

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
		 * \return	The list of entities.
		 */
		void GetAllEntities(EntityList &list) const;

		/**
		 * \brief	Gets the number of entities.
		 *
		 * \return	The entity count.
		 */
		EntityList::size_type GetEntityCount() const;

		/**
		 * \brief	Initialises the entity by calling Init()
		 * 				on all components.
		 * 				
		 * Safe to call multiple times. Will only work the first time.
		 *
		 * \param	id	The entity.
		 *
		 * \return	true if it succeeds, false if already called once.
		 */
		bool InitEntity(const EntityID &id);

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
		 * \brief	Gets an archetype.
		 *
		 * \param	name	The name.
		 *
		 * \return	null if it fails, else the archetype.
		 */
		const Archetype *GetArchetype(const std::string &name) const;

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
		 * \param	id				 	The identifier.
		 * \param	getExisting	(optional) If true, and if the
		 * 										component is already added this will
		 * 										return the existing component.
		 *
		 * \return	null or existing component if it fails, else the component.
		 */
		template<class T>
		T *AddComponent(const EntityID &id, bool getExisting = true)
		{
			return dynamic_cast<T*>(AddComponent(id, T::Family, getExisting));
		}

		/**
		 * \brief	Adds a component to 'id'.
		 *
		 * \param	id				 	The identifier.
		 * \param	family		 	The family of the component.
		 * \param	getExisting	(optional) If true, and if the
		 * 										component is already added this will
		 * 										return the existing component.
		 *
		 * \return	null or existing component if it fails, else the component.
		 */
		Component *AddComponent(const EntityID &id, const HashString &family, bool getExisting = true);

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
			return RemoveComponent(id, T::Family);
		}

		/**
		 * \brief	Removes the component.
		 *
		 * \param	id			The identifier.
		 * \param	family	The family of the component.
		 *
		 * \return	true if it succeeds, false if it fails.
		 */
		bool RemoveComponent(const EntityID &id, const HashString &family);

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
			return HasComponent(id, T::Family);
		}

		/**
		 * \brief	Query if 'id' has a component.
		 *
		 * \param	id			The identifier.
		 * \param	family	The family of the component.
		 *
		 * \return	true if it has the component, false if not.
		 */
		bool HasComponent(const EntityID &id, const HashString &family) const;

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
			return dynamic_cast<T*>(GetComponent(id, T::Family));
		}

		/**
		 * \brief	Gets a component.
		 *
		 * \param	id			The identifier.
		 * \param	family	The family of the component.
		 *
		 * \return	null if it fails, else the component.
		 */
		Component *GetComponent(const EntityID &id, const HashString &family) const;

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
			return GetEntitiesWith(T::Family);
		}

		/**
		 * \brief	Gets all components for an entity.
		 *
		 * \param	family	The family of the component.
		 *
		 * \return	The entities with the component.
		 */
		const EntityComponentMap &GetEntitiesWith(const HashString &family) const;

		/**
		 * \brief	Updates all components.
		 *
		 * \param	deltaTime	Time since last frame.
		 */
		void UpdateAll(float deltaTime);

	private:
		typedef ObjectFactory<Component*(), HashString> ComponentFactory;
		ComponentFactory factory;

		class Impl;
		Impl *p;
	};
}

#endif // PZ_EntityManager_h__
