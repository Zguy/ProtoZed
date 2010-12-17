/*
	Copyright 2010 Johannes H�ggqvist

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
#include "ProtoZed/AnimableDrawable.h"

namespace PZ
{
	AnimableDrawable::AnimableDrawable(sf::Drawable &drawable) : drawable(drawable)
	{

	}
	AnimableDrawable::~AnimableDrawable()
	{

	}

	bool AnimableDrawable::SupportAttribute(Attribute attribute)
	{
		bool support = false;
		switch (attribute)
		{
		case X        : support = true; break;
		case Y        : support = true; break;
		case SCALE_X  : support = true; break;
		case SCALE_Y  : support = true; break;
		case ROTATION : support = true; break;
		case COLOR_R  : support = true; break;
		case COLOR_G  : support = true; break;
		case COLOR_B  : support = true; break;
		case ALPHA    : support = true; break;
		}
		return support;
	}

	void AnimableDrawable::SetAttribute(Attribute attribute, float value)
	{
		if (attribute == X)
			drawable.SetPosition(value, drawable.GetPosition().y);
		else if (attribute == Y)
			drawable.SetPosition(drawable.GetPosition().x, value);
		else if (attribute == SCALE_X)
			drawable.SetScale(value, drawable.GetScale().y);
		else if (attribute == SCALE_Y)
			drawable.SetScale(drawable.GetScale().x, value);
		else if (attribute == ROTATION)
			drawable.SetRotation(value);
		else if ((attribute == COLOR_R)||(attribute == COLOR_G)||(attribute == COLOR_B)||(attribute == ALPHA))
		{
			sf::Color color = drawable.GetColor();
			if (attribute == COLOR_R)
				color.r = static_cast<sf::Uint8>(value);
			else if (attribute == COLOR_G)
				color.g = static_cast<sf::Uint8>(value);
			else if (attribute == COLOR_B)
				color.b = static_cast<sf::Uint8>(value);
			else if (attribute == ALPHA)
				color.a = static_cast<sf::Uint8>(value);
			drawable.SetColor(color);
		}
	}
	float AnimableDrawable::GetAttribute(Attribute attribute)
	{
		if (attribute == X)
			return drawable.GetPosition().x;
		else if (attribute == Y)
			return drawable.GetPosition().y;
		else if (attribute == SCALE_X)
			return drawable.GetScale().x;
		else if (attribute == SCALE_Y)
			return drawable.GetScale().y;
		else if (attribute == ROTATION)
			return drawable.GetRotation();
		else if (attribute == COLOR_R)
			return drawable.GetColor().r;
		else if (attribute == COLOR_G)
			return drawable.GetColor().g;
		else if (attribute == COLOR_B)
			return drawable.GetColor().b;
		else if (attribute == ALPHA)
			return drawable.GetColor().a;
		else
			return 0.f;
	}
}