/*
Copyright (c) 2012 Johannes H�ggqvist

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

		inline const std::string &GetSprite() const
		{
			return sprite;
		}
		inline void SetSprite(const std::string &newSprite)
		{
			sprite = newSprite;
		}

		inline const Vector2f GetCenter() const
		{
			return center;
		}
		inline void SetCenter(const Vector2f &newCenter)
		{
			center = newCenter;
		}

	private:
		Property<std::string> sprite;
		Property<Vector2f> center;
	};
}

#endif // PZ_Sprite_h__