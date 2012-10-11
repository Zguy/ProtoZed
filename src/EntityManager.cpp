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
#include <ProtoZed/EntityManager.h>

#include <ProtoZed/MetaEntity.h>
#include <ProtoZed/Component.h>
#include <ProtoZed/Archetype.h>
#include <ProtoZed/Log.h>

namespace PZ
{
	struct EntityData
	{
		EntityData() : destroy(false)
		{}

		bool destroy;
	};
	typedef std::unordered_map<EntityID, EntityData, std::hash<int>> EntityDataMap;

	typedef std::unordered_map<HashString, EntityComponentMap, std::hash<int>> ComponentStore;
	typedef std::unordered_map<HashString, Archetype*, std::hash<int>> ArchetypeMap;

	class EntityManager::Impl
	{
	public:
		Impl(Application *application) : application(application)
		{}

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

		Application *application;

		EntitySet entities;
		EntityDataMap entityDataMap;

		ComponentStore components;
		static const EntityComponentMap emptyMap;

		ArchetypeMap archetypes;
	};

	const EntityComponentMap EntityManager::Impl::emptyMap;

	EntityManager::EntityManager(Application &application) : p(new Impl(&application))
	{
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
			e.id = id;
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
				e.id = id;
				EmitEvent(e);
			}
			else
			{
				++it;
			}
		}
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

	void EntityManager::GetAllRegisteredComponents(std::vector<HashString> &list) const
	{
		list.clear();

		for (ComponentFactory::ConstIterator it = factory.GetBegin(); it != factory.GetEnd(); ++it)
		{
			list.push_back((*it).first);
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
			for (EntityComponentMap::const_iterator it2 = ecm.cbegin(); it2 != ecm.cend(); ++it2)
			{
				if ((*it2).first == id)
				{
					list.push_back((*it).first);
				}
			}
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

	bool EntityManager::AddComponentImpl(const EntityID &id, const HashString &name, Component *component)
	{
		if (!HasEntity(id) || HasComponentImpl(id, name))
			return false;

		component->application = p->application;
		component->owner = id;
		component->manager = this;

		bool success = p->components[name].insert(std::make_pair(id, component)).second;

		component->Init();

		ComponentEvent e(ComponentEvent::ADDED);
		e.id = id;
		e.family = name;
		EmitEvent(e);

		return success;
	}
	bool EntityManager::RemoveComponentImpl(const EntityID &id, const HashString &name)
	{
		ComponentStore::iterator it = p->components.find(name);
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
				(*it2).second->Destroy();

				p->eraseComponent(it, it2);

				ComponentEvent e(ComponentEvent::REMOVED);
				e.id = id;
				e.family = name;
				EmitEvent(e);

				return true;
			}
		}
	}
	bool EntityManager::HasComponentImpl(const EntityID &id, const HashString &name) const
	{
		ComponentStore::iterator it = p->components.find(name);
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
	Component *EntityManager::GetComponentImpl(const EntityID &id, const HashString &name) const
	{
		ComponentStore::iterator it = p->components.find(name);
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
	const EntityComponentMap &EntityManager::GetEntitiesWithImpl(const HashString &name) const
	{
		ComponentStore::const_iterator it = p->components.find(name);
		if (it == p->components.end())
		{
			return p->emptyMap;
		}
		else
		{
			return (*it).second;
		}
	}

	void EntityManager::deleteComponent(Component *component)
	{
		delete component;
	}
}