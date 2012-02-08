/*
Copyright (c) 2012 Johannes Häggqvist

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/
#ifndef SpritesheetEntity_h__
#define SpritesheetEntity_h__

#include <ProtoZed/Component.h>
#include <ProtoZed/Attributes.h>
#include <ProtoZed/Vector2.h>

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
		PZ::Vector2i size;
		SpritesheetAnimationDefinitionList animations;
	};

	class SpritesheetComponent : public Component
	{
	public:
		SpritesheetComponent(const EntityID &owner, EntityManager &manager);
		~SpritesheetComponent();

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
		virtual bool HasAttribute(Attribute attribute) const;
		virtual void SetAttribute(Attribute attribute, float value);
		virtual float GetAttribute(Attribute attribute) const;

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