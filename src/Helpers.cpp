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
			const float rotRad = Convert::DegreesToRadians(rotation);
			return sf::Vector2f(std::cos(rotRad) * length * -1,
													std::sin(rotRad) * length);
		}

		float GetDistance(const sf::Vector2f &pointA, const sf::Vector2f &pointB)
		{
			return std::sqrt(GetDistanceSquared(pointA, pointB));
		}
		float GetDistanceSquared(const sf::Vector2f &pointA, const sf::Vector2f &pointB)
		{
			const float x = std::abs(pointB.x - pointA.x);
			const float y = std::abs(pointB.y - pointA.y);

			return (x*x + y*y);
		}

		float GetRotationTo(const sf::Vector2f &from, const sf::Vector2f &to)
		{
			const float x = to.x - from.x;
			const float y = to.y - from.y;

			const float angle = Convert::RadiansToDegrees(std::atan2(x, y)) + 180.f;

			return angle;
		}
	}
}