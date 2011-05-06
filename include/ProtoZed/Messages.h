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
#ifndef Messages_h__
#define Messages_h__

#include <ProtoZed/Message.h>

#include <SFML/Graphics/RenderWindow.hpp>

namespace PZ
{
	namespace MessageID
	{
		enum Type
		{
			UPDATE = 1,
			POSITION_UPDATED,
			ROTATION_UPDATED,
			DRAW,
			USER // Start your own message ids from here
		};
	}

	struct UpdateMessage : public Message
	{
		UpdateMessage(float deltaTime) : Message(MessageID::UPDATE), deltaTime(deltaTime)
		{}

		float deltaTime;
	};

	struct DrawMessage : public Message
	{
		DrawMessage(sf::RenderWindow &window) : Message(MessageID::DRAW), window(window)
		{}

		sf::RenderWindow &window;
	};
}

#endif // Messages_h__