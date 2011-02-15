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
#include <ProtoZed/Helpers.h>

#include <ProtoZed/Convert.h>

#include <cmath>

namespace PZ
{
	namespace Helpers
	{
		sf::Vector2f GetForwardVector(float rotation, float length)
		{
			return sf::Vector2f(std::cos(Convert::DegreesToRadians(rotation)) * length * -1,
													std::sin(Convert::DegreesToRadians(rotation)) * length);
		}

		float GetDistance(const sf::Vector2f &pointA, const sf::Vector2f &pointB)
		{
			return std::sqrt(GetDistanceSquared(pointA, pointB));
		}
		float GetDistanceSquared(const sf::Vector2f &pointA, const sf::Vector2f &pointB)
		{
			float x = std::abs(pointB.x - pointA.x);
			float y = std::abs(pointB.y - pointA.y);

			return (x*x + y*y);
		}

		float GetRotationTo(const sf::Vector2f &from, const sf::Vector2f &to)
		{
			float x = to.x - from.x;
			float y = to.y - from.y;

			float angle = Convert::RadiansToDegrees(std::atan2(x, y)) + 180.f;

			return angle;
		}
	}
}