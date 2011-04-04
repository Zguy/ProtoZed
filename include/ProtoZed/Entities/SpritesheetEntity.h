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
#ifndef SpritesheetEntity_h__
#define SpritesheetEntity_h__

#include <ProtoZed/Entity.h>
#include <ProtoZed/Animation/Animable.h>

#include <SFML/Graphics/Sprite.hpp>

namespace PZ
{
	struct SpritesheetAnimationDefinition
	{
		std::string name;
		unsigned int from, to;
		float time;
		bool loop;
	};
	typedef std::vector<SpritesheetAnimationDefinition> SpritesheetAnimationDefinitionList;
	struct SpritesheetDefinition
	{
		std::string name;
		std::string image;
		sf::Vector2i size;
		SpritesheetAnimationDefinitionList animations;
	};

	class SpritesheetEntity : public Entity, public Animable
	{
	public:
		SpritesheetEntity(const std::string &name);
		virtual ~SpritesheetEntity();

		void SetSpritesheet(const SpritesheetDefinition &spritesheet);
		bool SetSpritesheetFromFile(const std::string &filename);

		bool SetAnimation(const std::string &name);
		const std::string &GetAnimation() const;

		// Pretend we're a sf::Sprite
		inline void SetScale(float ScaleX, float ScaleY) { sprite.SetScale(ScaleX, ScaleY); }
		inline void SetScale(const sf::Vector2f& Scale) { sprite.SetScale(Scale); }

		inline void SetScaleX(float FactorX) { sprite.SetScaleX(FactorX); }
		inline void SetScaleY(float FactorY) { sprite.SetScaleY(FactorY); }

		inline void SetCenter(float CenterX, float CenterY) { sprite.SetCenter(CenterX, CenterY); }
		inline void SetCenter(const sf::Vector2f& Center) { sprite.SetCenter(Center); }

		inline void SetColor(const sf::Color& Col) { sprite.SetColor(Col); }

		inline void SetBlendMode(sf::Blend::Mode Mode) { sprite.SetBlendMode(Mode); }

		inline const sf::Vector2f& GetScale() const { return sprite.GetScale(); }

		inline const sf::Vector2f& GetCenter() const { return sprite.GetCenter(); }

		inline const sf::Color& GetColor() const { return sprite.GetColor(); }

		inline sf::Blend::Mode GetBlendMode() const { return sprite.GetBlendMode(); }

		inline sf::Vector2f GetSize() const { return sprite.GetSize(); }

		inline void Scale(float FactorX, float FactorY) { sprite.Scale(FactorX, FactorY); }
		inline void Scale(const sf::Vector2f& Factor) { sprite.Scale(Factor); }

		inline void Resize(float Width, float Height) { sprite.Resize(Width, Height); }
		inline void Resize(const sf::Vector2f& Size) { sprite.Resize(Size); }

		// Animable
		virtual bool HasAttribute(Attribute attribute);
		virtual void SetAttribute(Attribute attribute, float value);
		virtual float GetAttribute(Attribute attribute) const;

	protected:
		virtual bool HandleMessage(Message &message);

	private:
		void setFrame(unsigned int frame);

		sf::Sprite sprite;
		SpritesheetDefinition spritesheetDef;
		unsigned int animationIndex;
		unsigned int currentFrame;
		float currentTime;
	};
}

#endif // SpritesheetEntity_h__