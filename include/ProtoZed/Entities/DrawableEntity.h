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
#ifndef DrawableEntity_h__
#define DrawableEntity_h__

#include <ProtoZed/Entity.h>
#include <ProtoZed/Animation/Animable.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Drawable.hpp>

namespace PZ
{
	namespace MessageID
	{
		static const std::string DRAW = "Draw";
	}

	struct DrawMessage : public Message
	{
		DrawMessage(sf::RenderWindow &window) : Message(MessageID::DRAW), window(window)
		{}

		sf::RenderWindow &window;
	};

	namespace Attributes
	{
		static const Animable::Attribute X        = "X";
		static const Animable::Attribute Y        = "Y";
		static const Animable::Attribute SCALE_X  = "SCALE_X";
		static const Animable::Attribute SCALE_Y  = "SCALE_Y";
		static const Animable::Attribute ROTATION = "ROTATION";
		static const Animable::Attribute COLOR_R  = "COLOR_R";
		static const Animable::Attribute COLOR_G  = "COLOR_G";
		static const Animable::Attribute COLOR_B  = "COLOR_B";
		static const Animable::Attribute ALPHA    = "ALPHA";
	}

	class DrawableEntity : public Entity, public Animable
	{
	public:
		DrawableEntity(const std::string &name);
		virtual ~DrawableEntity();

		inline bool HasDrawable() const { return (drawable != NULL); }
		inline void SetDrawable(sf::Drawable *newDrawable) { drawable = newDrawable; drawable->SetPosition(GetGlobalPosition()); }
		inline sf::Drawable *GetDrawable() const { return drawable; }

		// Pretend we're a sf::Drawable
		// Note: We should probably check if drawable is NULL
		//       in every function, but I can't be bothered.
		inline void SetScale(float ScaleX, float ScaleY) { drawable->SetScale(ScaleX, ScaleY); }
		inline void SetScale(const sf::Vector2f& Scale) { drawable->SetScale(Scale); }

		inline void SetScaleX(float FactorX) { drawable->SetScaleX(FactorX); }
		inline void SetScaleY(float FactorY) { drawable->SetScaleY(FactorY); }

		inline void SetCenter(float CenterX, float CenterY) { drawable->SetCenter(CenterX, CenterY); }
		inline void SetCenter(const sf::Vector2f& Center) { drawable->SetCenter(Center); }

		inline void SetRotation(float Rotation) { drawable->SetRotation(Rotation); }

		inline void SetColor(const sf::Color& Col) { drawable->SetColor(Col); }

		inline void SetBlendMode(sf::Blend::Mode Mode) { drawable->SetBlendMode(Mode); }

		inline const sf::Vector2f& GetScale() const { return drawable->GetScale(); }

		inline const sf::Vector2f& GetCenter() const { return drawable->GetCenter(); }

		inline float GetRotation() const { return drawable->GetRotation(); }

		inline const sf::Color& GetColor() const { return drawable->GetColor(); }

		inline sf::Blend::Mode GetBlendMode() const { return drawable->GetBlendMode(); }

		inline void Scale(float FactorX, float FactorY) { drawable->Scale(FactorX, FactorY); }
		inline void Scale(const sf::Vector2f& Factor) { drawable->Scale(Factor); }

		inline void Rotate(float Angle) { drawable->Rotate(Angle); }

		// Animable
		virtual bool HasAttribute(Attribute attribute);
		virtual void SetAttribute(Attribute attribute, float value);
		virtual float GetAttribute(Attribute attribute) const;

	protected:
		virtual bool OnMessage(MessagePtr message);

	private:
		sf::Drawable *drawable;
	};

	WEAK_SHARED_PTR(DrawableEntity)
}

#endif // DrawableEntity_h__