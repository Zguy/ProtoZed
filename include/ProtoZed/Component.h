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
#ifndef PZ_Component_h__
#define PZ_Component_h__

#include <ProtoZed/NonCopyable.h>
#include <ProtoZed/PropertyList.h>
#include <ProtoZed/EntityManager.h>
#include <ProtoZed/Timestamp.h>
#include <ProtoZed/MetaEntity.h>
#include <ProtoZed/Message.h>

#include <string>

namespace PZ
{
	/**
	 * \brief	Small reusable block of behavior.
	 */
	class Component : public NonCopyable, public PropertyList
	{
		friend class EntityManager;

	protected:

		/**
		 * \brief	Default constructor.
		 * 
		 * Don't put any initialization logic inside
		 * the constructor (anything that will call GetManager() or GetOwnerID()).
		 * Use Init() instead.
		 */
		Component() : manager(nullptr)
		{}

	public:
		virtual ~Component()
		{}

		/**
		 * \brief	Initializes the component.
		 * 
		 * You should not call this method.
		 * It will be called by EntityManager.
		 */
		virtual void Init()
		{}

		/**
		 * \brief	Destroys the component.
		 * 
		 * This is provided for consistency with Init().
		 * It is okay to put destruction logic inside the destructor.
		 * 
		 * You should not call this method.
		 * It will be called by EntityManager.
		 */
		virtual void Destroy()
		{}

		/**
		 * \brief	Runs an update.
		 *
		 * \param	deltaTime	Time since the last frame.
		 */
		virtual void Update(float deltaTime)
		{}

		/**
		 * \brief	Handles a message.
		 *
		 * \param	message	The message.
		 *
		 * \return	true if the message was handled.
		 */
		virtual bool HandleMessage(const Message &message)
		{
			return false;
		}

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
