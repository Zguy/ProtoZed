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

		static const std::string SPRITE;
		static const std::string CENTER;

		Sprite(const EntityID &owner, EntityManager &manager) : Component(owner, manager)
		{
			AddProperty(SPRITE, Property::STRING);
			AddProperty(CENTER, Property::VECTOR2);
		}
		~Sprite()
		{
		}

		inline const std::string &GetSprite() const
		{
			return GetProperty(SPRITE).AsString();
		}
		inline void SetSprite(const std::string &newSprite)
		{
			GetProperty(SPRITE) = newSprite;
		}

		inline const Vector2f GetCenter() const
		{
			return GetProperty(CENTER).AsVector2();
		}
		inline void SetCenter(const Vector2f &newCenter)
		{
			GetProperty(CENTER) = newCenter;
		}
	};
}

#endif // PZ_Sprite_h__