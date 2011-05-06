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
#ifndef Message_h__
#define Message_h__

#include <string>

namespace PZ
{
	typedef unsigned int MessageType;

	struct Message
	{
		enum Mode
		{
			STAY,  // Don't float or sink to any children
			FLOAT, // Go to the leaf child and float up to the surface
			SINK   // Sink down the leaf child
		};

		Message(MessageType message = 0, Mode mode = SINK) : message(message), mode(mode)
		{}

		MessageType message;

		Mode mode;

		template<class T>
		T &As()
		{
			return static_cast<T&>(*this);
		}
		template<class T>
		const T &As() const
		{
			return static_cast<const T&>(*this);
		}
	};
}

#endif // Message_h__