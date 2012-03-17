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
	class Component : public NonCopyable, public PropertyList
	{
		friend class EntityManager;

	protected:
		Component(const EntityID &owner, EntityManager &manager) : owner(owner), manager(manager)
		{}
		virtual ~Component()
		{}

	public:
		virtual bool HandleMessage(const Message &message)
		{
			return false;
		}

		EntityID GetOwnerID() const
		{
			return owner;
		}
		MetaEntity GetOwnerEntity() const
		{
			return manager.GetEntity(owner);
		}
		EntityManager &GetManager() const
		{
			return manager;
		}

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

		EntityManager &manager;

		Timestamp stamp;
	};
}

#endif // PZ_Component_h__