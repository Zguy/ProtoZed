/*
	Copyright 2010-2011 Johannes H�ggqvist

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
#ifndef IncludeFilter_h__
#define IncludeFilter_h__

namespace PZ
{
	class Entity;

	class IncludeFilter
	{
	public:
		IncludeFilter()
		{}
		virtual ~IncludeFilter()
		{}

		virtual bool TestEntity(const Entity *entity) const = 0;
	};
}

#endif // IncludeFilter_h__