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
#include <ProtoZed/Log.h>

#include <vector>

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
	typedef std::vector<EntityListener*> ListenerList;

	class EntityManager::Impl
	{
	public:
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

		// Listener emits
		void emitEntityCreatedPre(const EntityID &id)
		{
			for (ListenerList::iterator it = listeners.begin(); it != listeners.end(); ++it)
			{
				(*it)->EntityCreatedPre(id);
			}
		}
		void emitEntityCreatedPost(const EntityID &id)
		{
			for (ListenerList::iterator it = listeners.begin(); it != listeners.end(); ++it)
			{
				(*it)->EntityCreatedPost(id);
			}
		}
		void emitEntityDestroyedPre(const EntityID &id)
		{
			for (ListenerList::iterator it = listeners.begin(); it != listeners.end(); ++it)
			{
				(*it)->EntityDestroyedPre(id);
			}
		}
		void emitEntityDestroyedPost(const EntityID &id)
		{
			for (ListenerList::iterator it = listeners.begin(); it != listeners.end(); ++it)
			{
				(*it)->EntityDestroyedPost(id);
			}
		}

		void emitEntitiesClearedPre()
		{
			for (ListenerList::iterator it = listeners.begin(); it != listeners.end(); ++it)
			{
				(*it)->EntitiesClearedPre();
			}
		}
		void emitEntitiesClearedPost()
		{
			for (ListenerList::iterator it = listeners.begin(); it != listeners.end(); ++it)
			{
				(*it)->EntitiesClearedPost();
			}
		}

		void emitComponentAddedPre(const EntityID &id, const HashString &family)
		{
			for (ListenerList::iterator it = listeners.begin(); it != listeners.end(); ++it)
			{
				(*it)->ComponentAddedPre(id, family);
			}
		}
		void emitComponentAddedPost(const EntityID &id, const HashString &family)
		{
			for (ListenerList::iterator it = listeners.begin(); it != listeners.end(); ++it)
			{
				(*it)->ComponentAddedPost(id, family);
			}
		}
		void emitComponentRemovedPre(const EntityID &id, const HashString &family)
		{
			for (ListenerList::iterator it = listeners.begin(); it != listeners.end(); ++it)
			{
				(*it)->ComponentRemovedPre(id, family);
			}
		}
		void emitComponentRemovedPost(const EntityID &id, const HashString &family)
		{
			for (ListenerList::iterator it = listeners.begin(); it != listeners.end(); ++it)
			{
				(*it)->ComponentRemovedPost(id, family);
			}
		}

		EntityList entities;
		EntityDataMap entityDataMap;

		ComponentStore components;
		static const EntityComponentMap emptyMap; //TODO: This solution sucks

		ListenerList listeners;
	};

	const EntityComponentMap EntityManager::Impl::emptyMap;

	EntityManager::EntityManager() : p(new Impl)
	{
	}
	EntityManager::~EntityManager()
	{
		ClearEntities();
		DestroyPendingEntities();

		delete p;
	}

	bool EntityManager::CreateEntity(const EntityID &id)
	{
		if (!HasEntity(id))
		{
			p->emitEntityCreatedPre(id);

			p->entities.push_back(id);
			p->entityDataMap.insert(std::make_pair(id, EntityData()));

			p->emitEntityCreatedPost(id);

			return true;
		}
		else
		{
			return false;
		}
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
		for (EntityList::iterator it = p->entities.begin(); it != p->entities.end();)
		{
			if (p->getDataFor(*it).destroy)
			{
				const EntityID id = (*it);

				p->emitEntityDestroyedPre(id);

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

				p->emitEntityDestroyedPost(id);
			}
			else
			{
				++it;
			}
		}
	}

	bool EntityManager::HasEntity(const EntityID &id) const
	{
		if (id == EntityID())
			return false;

		for (EntityList::const_iterator it = p->entities.begin(); it != p->entities.end(); ++it)
		{
			if (id == (*it))
			{
				return true;
			}
		}
		return false;
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
		p->emitEntitiesClearedPre();

		for (EntityList::const_iterator it = p->entities.cbegin(); it != p->entities.cend(); ++it)
		{
			p->getDataFor(*it).destroy = true;
		}

		p->emitEntitiesClearedPost();
	}

	const EntityList &EntityManager::GetAllEntities() const
	{
		return p->entities;
	}

	EntityList::size_type EntityManager::GetEntityCount() const
	{
		return p->entities.size();
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

	void EntityManager::SendMessageToAll(const Message &message) const
	{
		for (ComponentStore::const_iterator it = p->components.cbegin(); it != p->components.cend(); ++it)
		{
			const EntityComponentMap &ecm = (*it).second;
			for (EntityComponentMap::const_iterator it2 = ecm.cbegin(); it2 != ecm.cend(); ++it2)
			{
				Component *component = (*it2).second;
				component->HandleMessage(message);
			}
		}
	}
	bool EntityManager::SendMessage(const Message &message, const EntityID &to) const
	{
		// This is going to be slow..
		
		if (HasEntity(to))
		{
			for (ComponentStore::const_iterator it = p->components.cbegin(); it != p->components.cend(); ++it)
			{
				const EntityComponentMap &ecm = (*it).second;
				for (EntityComponentMap::const_iterator it2 = ecm.cbegin(); it2 != ecm.cend(); ++it2)
				{
					const EntityID &id = (*it2).first;
					Component *component = (*it2).second;

					if (id == to)
					{
						component->HandleMessage(message);
						break;
					}
				}
			}

			return true;
		}

		return false;
	}

	void EntityManager::RegisterListener(EntityListener *listener)
	{
		if (listener != nullptr)
		{
			p->listeners.push_back(listener);
		}
	}
	void EntityManager::UnregisterListener(EntityListener *listener)
	{
		for (ListenerList::iterator it = p->listeners.begin(); it != p->listeners.end(); ++it)
		{
			if ((*it) == listener)
			{
				p->listeners.erase(it);
				break;
			}
		}
	}

	bool EntityManager::AddComponentImpl(const EntityID &id, const HashString &name, Component *component)
	{
		if (!HasEntity(id) || HasComponentImpl(id, name))
			return false;

		p->emitComponentAddedPre(id, name);

		component->owner = id;
		component->manager = this;

		bool success = p->components[name].insert(std::make_pair(id, component)).second;

		component->Init();

		p->emitComponentAddedPost(id, name);

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
				p->emitComponentRemovedPre(id, name);

				(*it2).second->Destroy();

				p->eraseComponent(it, it2);

				p->emitComponentRemovedPost(id, name);

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
			// Help?
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