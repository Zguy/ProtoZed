/*
Copyright (c) 2013 Johannes H�ggqvist

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
#ifndef PZ_Component_h__
#define PZ_Component_h__

#include <ProtoZed/PropertyList.h>
#include <ProtoZed/EventHandler.h>
#include <ProtoZed/EntityManager.h>
#include <ProtoZed/Timestamp.h>
#include <ProtoZed/MetaEntity.h>

#include <boost/noncopyable.hpp>

namespace PZ
{
	/**
	 * \brief	Small reusable block of behavior.
	 */
	class Component : boost::noncopyable, public PropertyList, public EventHandler
	{
		friend class EntityManager;

	protected:

		/**
		 * \brief	Default constructor.
		 * 
		 * Don't put any initialization logic inside
		 * the constructor.
		 * Use Init() instead.
		 */
		Component() : manager(nullptr)
		{}

	public:
		virtual ~Component()
		{}

		/**
		 * \brief	Initializes the component.
		 */
		virtual void Init()
		{}

		/**
		 * \brief	Runs an update.
		 *
		 * \param	deltaTime	Time since the last frame.
		 */
		virtual void Update(float deltaTime)
		{}

		Application &GetApplication() const
		{
			assert(application != nullptr);
			return *application;
		}

		EntityID GetOwnerID() const
		{
			return owner;
		}
		MetaEntity GetOwnerEntity() const
		{
			assert(manager != nullptr);
			return manager->GetEntity(owner);
		}
		EntityManager &GetManager() const
		{
			assert(manager != nullptr);
			return *manager;
		}

		/**
		 * \brief	Gets the timestamp for when this component was last updated.
		 * 
		 * This can be used to do an update only when the component has changed.
		 *
		 * \return	The timestamp.
		 */
		const Timestamp &GetTimestamp() const
		{
			return stamp;
		}

		/**
		 * \brief	Gets another component from the owner entity.
		 *
		 * \param	Family	The family of the component.
		 *
		 * \return	null if it fails, else the component.
		 */
		Component *GetComponent(const HashString &family) const
		{
			Component *component = GetManager().GetComponent(GetOwnerID(), family);
			return component;
		}
		
		/**
		 * \brief	Gets another component from the owner entity.
		 *
		 * \return	null if it fails, else the component.
		 */
		template<class T>
		T *GetComponent() const
		{
			return dynamic_cast<T*>(GetComponent(T::Family));
		}

	protected:
		void UpdateTimestamp()
		{
			stamp.Now();
		}

		virtual void PropertyUpdated(const PropertyBase &prop)
		{
			UpdateTimestamp();
		}

	private:
		EntityID owner;

		Application *application;
		EntityManager *manager;

		Timestamp stamp;
	};
}

#endif // PZ_Component_h__
