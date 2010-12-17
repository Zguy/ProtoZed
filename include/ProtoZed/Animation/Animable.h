/*
	Copyright 2010 Johannes Häggqvist

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
#ifndef Animable_h__
#define Animable_h__

#include <memory>

namespace PZ
{
	class Animable
	{
	public:
		typedef unsigned int Attribute;

		Animable()
		{}
		~Animable()
		{}

		virtual bool SupportAttribute(Attribute attribute) = 0;

		virtual void SetAttribute(Attribute attribute, float value) = 0;
		virtual float GetAttribute(Attribute attribute) = 0;
	};

	typedef std::shared_ptr<Animable> AnimablePtr;
}

#endif // Animable_h__