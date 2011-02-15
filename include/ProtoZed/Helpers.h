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
#ifndef Helpers_h__
#define Helpers_h__

#include <SFML/System/Vector2.hpp>

namespace PZ
{
	namespace Helpers
	{
		sf::Vector2f GetForwardVector(float rotation, float length);

		float GetDistance(const sf::Vector2f &pointA, const sf::Vector2f &pointB);
		float GetDistanceSquared(const sf::Vector2f &pointA, const sf::Vector2f &pointB);

		float GetRotationTo(const sf::Vector2f &from, const sf::Vector2f &to);
	}
}

#endif // Helpers_h__