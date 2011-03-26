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

#include <ProtoZed/Entity.h>

namespace PZ
{
	class Component
	{
	public:
		Component(const std::string &name);
		~Component();
	
		inline const std::string &GetName() const { return name; }

		inline bool HasOwner() const { return (owner != NULL); }
		inline EntityPtr GetOwner() const { return owner; }
	
		void HandleMessage(Message &message);
	
	private:
		std::string name;

		EntityPtr owner;
	};
}

#endif // Component_h__