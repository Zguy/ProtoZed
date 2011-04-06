/*
	Copyright 2010-2011 Johannes Häggqvist

	This file is part of ProtoZed.

	ProtoZed is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	ProtoZed is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with ProtoZed.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef Component_h__
#define Component_h__

#include <ProtoZed/Animation/Animable.h>
#include <ProtoZed/Entity.h>
#include <ProtoZed/Message.h>

#include <string>

namespace PZ
{
	class Component : public Animable
	{
		friend class Entity;

	public:
		Component(const std::string &name) : name(name), owner(NULL)
		{}
		virtual ~Component()
		{}
	
		inline const std::string &GetName() const { return name; }

		inline bool HasOwner() const { return (owner != NULL); }
		inline Entity *GetOwner() const { return owner; }
	
		virtual bool HasAttribute(Attribute attribute) const { return false; }
		virtual void SetAttribute(Attribute attribute, float value) { }
		virtual float GetAttribute(Attribute attribute) const { return 0.f; }

		virtual bool ReceiveMessage(Message &message) = 0;
	
	private:
		std::string name;

		Entity *owner;
	};
}

#endif // Component_h__