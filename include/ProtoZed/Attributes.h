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
#ifndef Attributes_h__
#define Attributes_h__

#include <ProtoZed/Animation/Animable.h>

namespace PZ
{
	namespace Attributes
	{
		enum Type
		{
			X = 1, Y,
			SCALE_X, SCALE_Y,
			ROTATION,
			COLOR_R, COLOR_G, COLOR_B, ALPHA,

			VOLUME,

			USER // Start your own attributes from here
		};
	}
}

#endif // Attributes_h__