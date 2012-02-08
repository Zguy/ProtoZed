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
#ifndef PZ_Sprite_h__
#define PZ_Sprite_h__

#include <ProtoZed/Component.h>
#include <ProtoZed/Vector2.h>

namespace PZ
{
	class Sprite : public Component
	{
	public:
		static const HashString Family;

		Sprite(const EntityID &owner, EntityManager &manager);
		~Sprite();

		// Pretend we're a sf::Drawable
		// Note: We should probably check if drawable is NULL
		//       in every function, but I can't be bothered.
		/*inline void SetScale(float ScaleX, float ScaleY) { drawable->SetScale(ScaleX, ScaleY); }
		inline void SetScale(const sf::Vector2f& Scale) { drawable->SetScale(Scale); }

		inline void SetScaleX(float FactorX) { drawable->SetScaleX(FactorX); }
		inline void SetScaleY(float FactorY) { drawable->SetScaleY(FactorY); }

		inline void SetCenter(float CenterX, float CenterY) { drawable->SetCenter(CenterX, CenterY); }
		inline void SetCenter(const sf::Vector2f& Center) { drawable->SetCenter(Center); }

		inline void SetColor(const sf::Color& Col) { drawable->SetColor(Col); }

		inline void SetBlendMode(sf::Blend::Mode Mode) { drawable->SetBlendMode(Mode); }

		inline const sf::Vector2f& GetScale() const { return drawable->GetScale(); }

		inline const sf::Vector2f& GetCenter() const { return drawable->GetCenter(); }

		inline const sf::Color& GetColor() const { return drawable->GetColor(); }

		inline sf::Blend::Mode GetBlendMode() const { return drawable->GetBlendMode(); }

		inline void Scale(float FactorX, float FactorY) { drawable->Scale(FactorX, FactorY); }
		inline void Scale(const sf::Vector2f& Factor) { drawable->Scale(Factor); }*/

		inline const std::string &GetSprite() const
		{
			return sprite;
		}
		inline void SetSprite(const std::string &newSprite)
		{
			sprite = newSprite;
		}

		inline const Vector2f &GetCenter() const
		{
			return center;
		}
		inline void SetCenter(const Vector2f &newCenter)
		{
			center = newCenter;
		}

	private:
		std::string sprite;
		Vector2f center;
	};
}

#endif // PZ_PZ_Sprite_h__