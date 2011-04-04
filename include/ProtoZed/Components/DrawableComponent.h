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
#ifndef DrawableComponent_h__
#define DrawableComponent_h__

#include <ProtoZed/Component.h>

#include <SFML/Graphics/Drawable.hpp>

namespace PZ
{
	class DrawableComponent : public Component
	{
	public:
		DrawableComponent();
		~DrawableComponent();

		inline bool HasDrawable() const { return (drawable != NULL); }
		inline void SetDrawable(sf::Drawable *newDrawable) { drawable = newDrawable; drawable->SetPosition(GetOwner()->GetGlobalPosition()); }
		inline sf::Drawable *GetDrawable() const { return drawable; }

		virtual bool ReceiveMessage(Message &message);

	private:
		sf::Drawable *drawable;
	};
}

#endif // DrawableComponent_h__