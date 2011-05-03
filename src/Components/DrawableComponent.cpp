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
#include <ProtoZed/Components/DrawableComponent.h>

namespace PZ
{
	DrawableComponent::DrawableComponent() : Component("Drawable"), drawable(NULL)
	{

	}
	DrawableComponent::~DrawableComponent()
	{
		if (HasDrawable())
			delete drawable;
	}

	void DrawableComponent::SetDrawable(sf::Drawable *newDrawable)
	{
		if (HasDrawable())
		{
			delete drawable;
			drawable = NULL;
		}

		drawable = newDrawable;
		
		if (HasDrawable() && HasOwner())
		{
			GetDrawable()->SetPosition(GetOwner()->GetGlobalPosition());
			GetDrawable()->SetRotation(GetOwner()->GetGlobalRotation());
		}
	}

	bool DrawableComponent::HasAttribute(Attribute attribute) const
	{
		return ((attribute == Attributes::SCALE_X)||
						(attribute == Attributes::SCALE_Y)||
						(attribute == Attributes::COLOR_R)||
						(attribute == Attributes::COLOR_G)||
						(attribute == Attributes::COLOR_B)||
						(attribute == Attributes::ALPHA)||
						Component::HasAttribute(attribute));
	}
	void DrawableComponent::SetAttribute(Attribute attribute, float value)
	{
		Component::SetAttribute(attribute, value);

		if (attribute == Attributes::SCALE_X)
			SetScaleX(value);
		else if (attribute == Attributes::SCALE_Y)
			SetScaleY(value);
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
	float DrawableComponent::GetAttribute(Attribute attribute) const
	{
		if (attribute == Attributes::SCALE_X)
			return GetScale().x;
		else if (attribute == Attributes::SCALE_Y)
			return GetScale().y;
		else if (attribute == Attributes::COLOR_R)
			return GetColor().r;
		else if (attribute == Attributes::COLOR_G)
			return GetColor().g;
		else if (attribute == Attributes::COLOR_B)
			return GetColor().b;
		else if (attribute == Attributes::ALPHA)
			return GetColor().a;
		else
			return Component::GetAttribute(attribute);
	}

	bool DrawableComponent::ReceiveMessage(Message &message)
	{
		if (message.message == MessageID::POSITION_UPDATED)
		{
			if (HasDrawable())
			{
				drawable->SetPosition(GetOwner()->GetGlobalPosition());
			}

			return true;
		}
		else if (message.message == MessageID::ROTATION_UPDATED)
		{
			if (HasDrawable())
			{
				drawable->SetRotation(GetOwner()->GetGlobalRotation());
				drawable->SetPosition(GetOwner()->GetGlobalPosition());
			}

			return true;
		}
		else if (message.message == MessageID::DRAW)
		{
			if (HasDrawable())
			{
				DrawMessage &drawMessage = message.As<DrawMessage>();
				drawMessage.window.Draw(*drawable);

				return true;
			}
		}

		return false;
	}

	void DrawableComponent::SetOwner(Entity *newOwner)
	{
		Component::SetOwner(newOwner);

		if (HasOwner() && HasDrawable())
		{
			GetDrawable()->SetPosition(GetOwner()->GetGlobalPosition());
		}
	}
}