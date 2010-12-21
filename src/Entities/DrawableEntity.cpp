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
#include "ProtoZed/Entities/DrawableEntity.h"

namespace PZ
{
	DrawableEntity::DrawableEntity(const std::string &name, Entity *parent) : Entity(name, parent), drawable(NULL)
	{

	}
	DrawableEntity::~DrawableEntity()
	{
 		if (HasDrawable())
 			delete drawable;
	}

	void DrawableEntity::SetAttribute(Attribute attribute, float value)
	{
		if (attribute == X)
			SetX(value);
		else if (attribute == Y)
			SetY(value);
		else if (attribute == SCALE_X)
			SetScaleX(value);
		else if (attribute == SCALE_Y)
			SetScaleY(value);
		else if (attribute == ROTATION)
			SetRotation(value);
		else if ((attribute == COLOR_R)||(attribute == COLOR_G)||(attribute == COLOR_B)||(attribute == ALPHA))
		{
			sf::Color color = GetColor();
			if (attribute == COLOR_R)
				color.r = static_cast<sf::Uint8>(value);
			else if (attribute == COLOR_G)
				color.g = static_cast<sf::Uint8>(value);
			else if (attribute == COLOR_B)
				color.b = static_cast<sf::Uint8>(value);
			else if (attribute == ALPHA)
				color.a = static_cast<sf::Uint8>(value);
			SetColor(color);
		}
	}
	float DrawableEntity::GetAttribute(Attribute attribute) const
	{
		if (attribute == X)
			return GetLocalPosition().x;
		else if (attribute == Y)
			return GetLocalPosition().y;
		else if (attribute == SCALE_X)
			return GetScale().x;
		else if (attribute == SCALE_Y)
			return GetScale().y;
		else if (attribute == ROTATION)
			return GetRotation();
		else if (attribute == COLOR_R)
			return GetColor().r;
		else if (attribute == COLOR_G)
			return GetColor().g;
		else if (attribute == COLOR_B)
			return GetColor().b;
		else if (attribute == ALPHA)
			return GetColor().a;
		else
			return 0.f;
	}

	bool DrawableEntity::OnMessage(MessagePtr message)
	{
		if (message->message == MessageID::POSITION_UPDATED)
		{
			drawable->SetPosition(GetGlobalPosition());

			return true;
		}
		else if (message->message == MessageID::DRAW)
		{
			if (HasDrawable())
			{
				DrawMessage *drawMessage = static_cast<DrawMessage*>(message.get());
				drawMessage->window.Draw(*drawable);

				return true;
			}
		}

		return false;
	}
}