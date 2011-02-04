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
#include <ProtoZed/Entities/DrawableEntity.h>

namespace PZ
{
	DrawableEntity::DrawableEntity(const std::string &name) : Entity(name), drawable(NULL)
	{
	}
	DrawableEntity::~DrawableEntity()
	{
 		if (HasDrawable())
 			delete drawable;
	}

	bool DrawableEntity::HasAttribute(Attribute attribute)
	{
		return ((attribute == Attributes::X)||
						(attribute == Attributes::Y)||
						(attribute == Attributes::SCALE_X)||
						(attribute == Attributes::SCALE_Y)||
						(attribute == Attributes::ROTATION)||
						(attribute == Attributes::COLOR_R)||
						(attribute == Attributes::COLOR_G)||
						(attribute == Attributes::COLOR_B)||
						(attribute == Attributes::ALPHA));
	}
	void DrawableEntity::SetAttribute(Attribute attribute, float value)
	{
		if (attribute == Attributes::X)
			SetX(value);
		else if (attribute == Attributes::Y)
			SetY(value);
		else if (attribute == Attributes::SCALE_X)
			SetScaleX(value);
		else if (attribute == Attributes::SCALE_Y)
			SetScaleY(value);
		else if (attribute == Attributes::ROTATION)
			SetRotation(value);
		else if ((attribute == Attributes::COLOR_R)||(attribute == Attributes::COLOR_G)||(attribute == Attributes::COLOR_B)||(attribute == Attributes::ALPHA))
		{
			sf::Color color = GetColor();
			if (attribute == Attributes::COLOR_R)
				color.r = static_cast<sf::Uint8>(value);
			else if (attribute == Attributes::COLOR_G)
				color.g = static_cast<sf::Uint8>(value);
			else if (attribute == Attributes::COLOR_B)
				color.b = static_cast<sf::Uint8>(value);
			else if (attribute == Attributes::ALPHA)
				color.a = static_cast<sf::Uint8>(value);
			SetColor(color);
		}
	}
	float DrawableEntity::GetAttribute(Attribute attribute) const
	{
		if (attribute == Attributes::X)
			return GetLocalPosition().x;
		else if (attribute == Attributes::Y)
			return GetLocalPosition().y;
		else if (attribute == Attributes::SCALE_X)
			return GetScale().x;
		else if (attribute == Attributes::SCALE_Y)
			return GetScale().y;
		else if (attribute == Attributes::ROTATION)
			return GetRotation();
		else if (attribute == Attributes::COLOR_R)
			return GetColor().r;
		else if (attribute == Attributes::COLOR_G)
			return GetColor().g;
		else if (attribute == Attributes::COLOR_B)
			return GetColor().b;
		else if (attribute == Attributes::ALPHA)
			return GetColor().a;
		else
			return 0.f;
	}

	bool DrawableEntity::OnMessage(MessagePtr message)
	{
		bool handled = Entity::OnMessage(message);

		if (message->message == MessageID::POSITION_UPDATED)
		{
			if (HasDrawable())
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

		return handled;
	}
}