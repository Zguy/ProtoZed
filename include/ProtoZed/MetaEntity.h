/*
Copyright (c) 2012 Johannes H�ggqvist

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
		inline EntityManager &GetManager() const
		{
			return *manager;
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
		Component *AddComponent(const HashString &family)
		{
			if (IsValid())
				return manager->AddComponent(id, family);
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
		bool RemoveComponent(const HashString &family)
		{
			if (IsValid())
				return manager->RemoveComponent(id, family);
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
		bool HasComponent(const HashString &family) const
		{
			if (IsValid())
				return manager->HasComponent(id, family);
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
		Component *GetComponent(const HashString &family) const
		{
			if (IsValid())
				return manager->GetComponent(id, family);
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

#endif // PZ_MetaEntity_h__