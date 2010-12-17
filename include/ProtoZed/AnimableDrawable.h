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