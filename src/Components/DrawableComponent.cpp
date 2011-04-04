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

	bool DrawableComponent::ReceiveMessage(Message &message)
	{
		if (message.message == MessageID::POSITION_UPDATED)
		{
			if (HasDrawable())
			{
				drawable->SetPosition(GetOwner()->GetGlobalPosition());
				drawable->SetRotation(GetOwner()->GetGlobalRotation());
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
}