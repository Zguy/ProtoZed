/*
Copyright (c) 2013 Johannes Häggqvist

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
#include <ProtoZed/EntityManager.h>

#include <ProtoZed/MetaEntity.h>
#include <ProtoZed/Component.h>
#include <ProtoZed/Archetype.h>
#include <ProtoZed/Log.h>

#include <ProtoZed/Events/EntityEvent.h>
#include <ProtoZed/Events/ComponentEvent.h>

namespace PZ
{
	struct EntityData
	{
		EntityData() : initialised(false), destroy(false)
		{}

		bool initialised;
		bool destroy;
	};
	typedef std::unordered_map<EntityID, EntityData, std::hash<int>> EntityDataMap;

	typedef std::unordered_map<HashString, EntityComponentMap, std::hash<int>> ComponentStore;
	typedef std::unordered_map<HashString, Archetype*, std::hash<int>> ArchetypeMap;

	class EntityManager::Impl
	{
	public:
		Impl(EntityManager *i, Application *application) : i(i), application(application)
		{}

		bool addComponent(const EntityID &id, const HashString &name, Component *component)
		{
			if (!i->HasEntity(id) || i->HasComponent(id, name))
				return false;

			component->application = application;
			component->owner = id;
			component->manager = i;

			if (components[name].insert(std::make_pair(id, component)).second)
			{
				ComponentEvent e(ComponentEvent::ADDED);
				e.Id = id;
				e.Family = name;
				i->EmitEvent(e);

				return true;
			}

			return false;
		}

		void eraseComponent(ComponentStore::iterator &it, EntityComponentMap::iterator &it2)
		{
			EntityComponentMap &ecm = (*it).second;

			delete (*it2).second;
			ecm.erase(it2);

			if (ecm.empty())
			{
				components.erase(it);
			}
		}

		EntityData &getDataFor(const EntityID &id)
		{
			return (*entityDataMap.find(id)).second;
		}

		EntityManager *i;
		Application *application;

		EntitySet entities;
		EntityDataMap entityDataMap;

		ComponentStore components;
		static const EntityComponentMap emptyMap;

		ArchetypeMap archetypes;
	};

	const EntityComponentMap EntityManager::Impl::emptyMap;

	EntityManager::EntityManager(Application &application)
	{
		p = new Impl(this, &application);
	}
	EntityManager::~EntityManager()
	{
		ClearEntities();
		DestroyPendingEntities();

		UnregisterAllArchetypes();

		delete p;
	}

	bool EntityManager::CreateEntity(const EntityID &id)
	{
		if (id == EntityID())
			return false;

		if (!HasEntity(id))
		{
			p->entities.insert(id);
			p->entityDataMap.insert(std::make_pair(id, EntityData()));

			EntityEvent e(EntityEvent::CREATED);
			e.Id = id;
			EmitEvent(e);

			return true;
		}

		return false;
	}
	bool EntityManager::CreateEntity(const EntityID &id, MetaEntity &entity)
	{
		if (CreateEntity(id))
		{
			entity = GetEntity(id);
			return true;
		}

		return false;
	}

	bool EntityManager::CreateFromArchetype(const std::string &name, const EntityID &id)
	{
		ArchetypeMap::const_iterator it = p->archetypes.find(name);
		if (it != p->archetypes.cend())
		{
			const Archetype *archetype = (*it).second;
			return archetype->Create(*this, id);
		}

		return false;
	}
	bool EntityManager::CreateFromArchetype(const std::string &name, const EntityID &id, MetaEntity &entity)
	{
		if (CreateFromArchetype(name, id))
		{
			entity = GetEntity(id);
			return true;
		}

		return false;
	}

	bool EntityManager::DestroyEntity(const EntityID &id)
	{
		if (HasEntity(id))
		{
			p->getDataFor(id).destroy = true;

			return true;
		}

		return false;
	}
	void EntityManager::DestroyPendingEntities()
	{
		for (EntitySet::iterator it = p->entities.begin(); it != p->entities.end();)
		{
			if (p->getDataFor(*it).destroy)
			{
				const EntityID id = (*it);

				// Remove all components associated with this entity
				for (ComponentStore::iterator it2 = p->components.begin(); it2 != p->components.end();)
				{
					EntityComponentMap &ecm = (*it2).second;

					ComponentStore::iterator next_it2 = it2;
					++next_it2;

					EntityComponentMap::iterator it3 = ecm.find(id);
					if (it3 != ecm.end())
					{
						p->eraseComponent(it2, it3);
					}

					it2 = next_it2;
				}

				p->entityDataMap.erase(id);
				it = p->entities.erase(it);

				EntityEvent e(EntityEvent::DESTROYED);
				e.Id = id;
				EmitEvent(e);
			}
			else
			{
				++it;
			}
		}
	}

	bool EntityManager::IsAlive(const EntityID &id) const
	{
		if (HasEntity(id))
		{
			return !p->getDataFor(id).destroy;
		}
		return false;
	}

	bool EntityManager::HasEntity(const EntityID &id) const
	{
		return (p->entities.find(id) != p->entities.cend());
	}

	MetaEntity EntityManager::GetEntity(const EntityID &id) const
	{
		if (HasEntity(id))
		{
			MetaEntity entity(id, const_cast<EntityManager&>(*this));

			return entity;
		}
		else
		{
			return MetaEntity();
		}
	}

	void EntityManager::ClearEntities()
	{
		for (EntitySet::const_iterator it = p->entities.cbegin(); it != p->entities.cend(); ++it)
		{
			p->getDataFor(*it).destroy = true;
		}

		EntityEvent e(EntityEvent::CLEARED);
		EmitEvent(e);
	}

	void EntityManager::GetAllEntities(EntityList &list) const
	{
		list.clear();

		for (EntitySet::const_iterator it = p->entities.cbegin(); it != p->entities.cend(); ++it)
		{
			list.push_back(*it);
		}
	}

	EntityList::size_type EntityManager::GetEntityCount() const
	{
		return p->entities.size();
	}

	bool EntityManager::InitEntity(const EntityID &id)
	{
		EntityData &data = p->getDataFor(id);

		if (!data.initialised)
		{
			for (ComponentStore::iterator it = p->components.begin(); it != p->components.end(); ++it)
			{
				EntityComponentMap &ecm = (*it).second;
				EntityComponentMap::iterator it2 = ecm.find(id);
				if (it2 != ecm.end())
				{
					(*it2).second->Init();
				}
			}

			data.initialised = true;
			return true;
		}

		return false;
	}

	bool EntityManager::RegisterArchetype(Archetype *archetype)
	{
		if (archetype != nullptr)
		{
			if (!p->archetypes.insert(std::make_pair(archetype->name, archetype)).second)
			{
				delete archetype;
			}
			else
			{
				return true;
			}
		}

		return false;
	}
	bool EntityManager::UnregisterArchetype(const std::string &name)
	{
		ArchetypeMap::iterator it = p->archetypes.find(name);
		if (it != p->archetypes.end())
		{
			delete (*it).second;
			(*it).second = nullptr;
			p->archetypes.erase(it);
			return true;
		}

		return false;
	}
	void EntityManager::UnregisterAllArchetypes()
	{
		for (ArchetypeMap::iterator it = p->archetypes.begin(); it != p->archetypes.end(); ++it)
		{
			delete (*it).second;
			(*it).second = nullptr;
		}
		p->archetypes.clear();
	}

	const Archetype *EntityManager::GetArchetype(const std::string &name) const
	{
		ArchetypeMap::const_iterator it = p->archetypes.find(name);
		if (it != p->archetypes.cend())
		{
			return (*it).second;
		}
		return nullptr;
	}

	void EntityManager::GetAllRegisteredComponents(std::vector<HashString> &list) const
	{
		list.clear();

		for (ComponentFactory::ConstIterator it = factory.GetBegin(); it != factory.GetEnd(); ++it)
		{
			list.push_back((*it).first);
		}
	}

	Component *EntityManager::AddComponent(const EntityID &id, const HashString &family, bool getExisting)
	{
		Component *component = factory.Create(family);

		if (component != nullptr)
		{
			if (p->addComponent(id, family, component))
			{
				return component;
			}
			else
			{
				delete component;
			}
		}

		if (getExisting)
		{
			return GetComponent(id, family);
		}
		else
		{
			return nullptr;
		}
	}
	bool EntityManager::RemoveComponent(const EntityID &id, const HashString &family)
	{
		ComponentStore::iterator it = p->components.find(family);
		if (it == p->components.end())
		{
			return false;
		}
		else
		{
			EntityComponentMap &ecm = (*it).second;
			EntityComponentMap::iterator it2 = ecm.find(id);
			if (it2 == ecm.end())
			{
				return false;
			}
			else
			{
				p->eraseComponent(it, it2);

				ComponentEvent e(ComponentEvent::REMOVED);
				e.Id = id;
				e.Family = family;
				EmitEvent(e);

				return true;
			}
		}
	}
	bool EntityManager::HasComponent(const EntityID &id, const HashString &family) const
	{
		ComponentStore::iterator it = p->components.find(family);
		if (it == p->components.end())
		{
			return false;
		}
		else
		{
			EntityComponentMap &ecm = (*it).second;
			EntityComponentMap::iterator it2 = ecm.find(id);
			if (it2 == ecm.end())
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	}
	Component *EntityManager::GetComponent(const EntityID &id, const HashString &family) const
	{
		ComponentStore::iterator it = p->components.find(family);
		if (it == p->components.end())
		{
			return nullptr;
		}
		else
		{
			EntityComponentMap &ecm = (*it).second;
			EntityComponentMap::iterator it2 = ecm.find(id);
			if (it2 == ecm.end())
			{
				return nullptr;
			}
			else
			{
				return (*it2).second;
			}
		}
	}

	void EntityManager::GetAllComponents(const EntityID &id, ComponentList &list) const
	{
		if (!HasEntity(id))
			return;

		list.clear();

		for (ComponentStore::const_iterator it = p->components.cbegin(); it != p->components.cend(); ++it)
		{
			const EntityComponentMap &ecm = (*it).second;
			EntityComponentMap::const_iterator it2 = ecm.find(id);
			if (it2 != ecm.cend())
			{
				list.push_back((*it).first);
			}
		}
	}

	const EntityComponentMap &EntityManager::GetEntitiesWith(const HashString &family) const
	{
		ComponentStore::const_iterator it = p->components.find(family);
		if (it == p->components.end())
		{
			return p->emptyMap;
		}
		else
		{
			return (*it).second;
		}
	}

	void EntityManager::UpdateAll(float deltaTime)
	{
		Profile profile("UpdateComponents");

		for (ComponentStore::const_iterator it = p->components.cbegin(); it != p->components.cend(); ++it)
		{
			const EntityComponentMap &ecm = (*it).second;
			for (EntityComponentMap::const_iterator it2 = ecm.cbegin(); it2 != ecm.cend(); ++it2)
			{
				Component *component = (*it2).second;
				component->Update(deltaTime);
			}
		}
	}
}