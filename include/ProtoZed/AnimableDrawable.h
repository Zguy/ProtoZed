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
#ifndef AnimableDrawable_h__
#define AnimableDrawable_h__

#include <SFML/Graphics/Drawable.hpp>

#include "Animation/Animable.h"

namespace PZ
{
	class AnimableDrawable : public Animable
	{
	public:
		enum
		{
			NONE,
			X,Y,
			ROTATION,
			SCALE_X,SCALE_Y,
			COLOR_R,COLOR_G,COLOR_B,ALPHA
		};

		AnimableDrawable(sf::Drawable &drawable);
		virtual ~AnimableDrawable();

		virtual bool SupportAttribute(Attribute attribute);

		virtual void SetAttribute(Attribute attribute, float value);
		virtual float GetAttribute(Attribute attribute);

		inline sf::Drawable &GetDrawable() { return drawable; }

	private:
		sf::Drawable &drawable;
	};
}

#endif // AnimableDrawable_h__