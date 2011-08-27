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
#ifndef DistanceFilter_h__
#define DistanceFilter_h__

#include <ProtoZed/IncludeFilter.h>

#include <SFML/System/Vector2.hpp>

namespace PZ
{
	class DistanceFilter : public IncludeFilter
	{
	public:
		DistanceFilter(const sf::Vector2f &origin, const float distance);
		virtual ~DistanceFilter();

		virtual bool TestEntity(const Entity *entity) const;

	private:
		sf::Vector2f origin;
		float distance;
	};
}

#endif // DistanceFilter_h__