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

#include <vector>

namespace PZ
{
	typedef std::unordered_map<HashString, EntityComponentMap, std::hash<unsigned int>> ComponentStore;

	class EntityManager::Impl
	{
	public:
		bool hasEntity(const EntityID &id)
		{
			for (EntityList::const_iterator it = entities.cbegin(); it != entities.cend(); ++it)
			{
				if (id == (*it))
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

		EntityList entities;

		ComponentStore components;

		static const EntityComponentMap emptyMap; //TODO: This solution sucks
	};

	const EntityComponentMap EntityManager::Impl::emptyMap;

	EntityManager::EntityManager() : p(new Impl)
	{
	}
	EntityManager::~EntityManager()
	{
		p->entities.clear();

		for (ComponentStore::iterator it = p->components.begin(); it != p->components.end(); ++it)
		{
			EntityComponentMap &ecm = (*it).second;

			for (EntityComponentMap::iterator it2 = ecm.begin(); it2 != ecm.end(); ++it2)
			{
				delete (*it2).second;
			}
		}
		p->components.clear();

		delete p;
	}

	bool EntityManager::CreateEntity(const EntityID &id)
	{
		if (!p->hasEntity(id))
		{
			p->entities.push_back(id);

			return true;
		}
		else
		{
			return false;
		}
	}
	bool EntityManager::DestroyEntity(const EntityID &id)
	{
		for (EntityList::iterator it = p->entities.begin(); it != p->entities.end(); ++it)
		{
			if ((*it) == id)
			{
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

				p->entities.erase(it);

				return true;
			}
		}
		return false;
	}

	MetaEntity EntityManager::GetEntity(const EntityID &id) const
	{
		Profile profile("GetEntity");
		if (p->hasEntity(id))
		{
			MetaEntity entity(id, const_cast<EntityManager&>(*this));

			return entity;
		}
		else
		{
			return MetaEntity();
		}
	}

	const EntityList &EntityManager::GetAllEntities() const
	{
		return p->entities;
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
		
		if (p->hasEntity(to))
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

	bool EntityManager::AddComponentImpl(const EntityID &id, const HashString &name, Component *component)
	{
		return p->components[name].insert(std::make_pair(id, component)).second;
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
				p->eraseComponent(it, it2);

				return true;
			}
		}
	}
	bool EntityManager::HasComponentImpl(const EntityID &id, const HashString &name)
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
	Component *EntityManager::GetComponentImpl(const EntityID &id, const HashString &name)
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
	const EntityComponentMap &EntityManager::GetEntitiesWithImpl(const HashString &name)
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
}