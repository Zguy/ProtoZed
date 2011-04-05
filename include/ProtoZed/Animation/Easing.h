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
#ifndef Easing_h__
#define Easing_h__

namespace PZ
{
	namespace Easing
	{
		enum Transition
		{
			LINEAR,
			SINE,
			QUINT,
			QUART,
			QUAD,
			EXPO,
			ELASTIC,
			CUBIC,
			CIRC,
			BOUNCE,
			BACK
		};
		enum Equation
		{
			EASE_IN,
			EASE_OUT,
			EASE_IN_OUT
		};

		class EasingBase
		{
		public:
			virtual float easeIn(float t, float b, float c, float d) const    = 0;
			virtual float easeOut(float t, float b, float c, float d) const   = 0;
			virtual float easeInOut(float t, float b, float c, float d) const = 0;
		};
		class Back : public EasingBase
		{
		public:
			virtual float easeIn(float t, float b, float c, float d) const;
			virtual float easeOut(float t, float b, float c, float d) const;
			virtual float easeInOut(float t, float b, float c, float d) const;
		};
		class Bounce : public EasingBase
		{
		public:
			virtual float easeIn(float t, float b, float c, float d) const;
			virtual float easeOut(float t, float b, float c, float d) const;
			virtual float easeInOut(float t, float b, float c, float d) const;
		};
		class Circ : public EasingBase
		{
		public:
			float easeIn(float t, float b, float c, float d) const;
			float easeOut(float t, float b, float c, float d) const;
			float easeInOut(float t, float b, float c, float d) const;
		};
		class Cubic : public EasingBase
		{
		public:
			float easeIn(float t, float b, float c, float d) const;
			float easeOut(float t, float b, float c, float d) const;
			float easeInOut(float t, float b, float c, float d) const;
		};
		class Elastic : public EasingBase
		{
		public:
			float easeIn(float t, float b, float c, float d) const;
			float easeOut(float t, float b, float c, float d) const;
			float easeInOut(float t, float b, float c, float d) const;
		};
		class Expo : public EasingBase
		{
		public:
			float easeIn(float t, float b, float c, float d) const;
			float easeOut(float t, float b, float c, float d) const;
			float easeInOut(float t, float b, float c, float d) const;
		};
		class Quad : public EasingBase
		{
		public:
			float easeIn(float t, float b, float c, float d) const;
			float easeOut(float t, float b, float c, float d) const;
			float easeInOut(float t, float b, float c, float d) const;
		};
		class Quart : public EasingBase
		{
		public:
			float easeIn(float t, float b, float c, float d) const;
			float easeOut(float t, float b, float c, float d) const;
			float easeInOut(float t, float b, float c, float d) const;
		};
		class Quint : public EasingBase
		{
		public :
			float easeIn(float t, float b, float c, float d) const;
			float easeOut(float t, float b, float c, float d) const;
			float easeInOut(float t, float b, float c, float d) const;
		};
		class Sine : public EasingBase
		{
		public :
			float easeIn(float t, float b, float c, float d) const;
			float easeOut(float t, float b, float c, float d) const;
			float easeInOut(float t, float b, float c, float d) const;
		};
		class Linear : public EasingBase
		{
		public :
			float easeNone(float t, float b, float c, float d) const;
			float easeIn(float t, float b, float c, float d) const;
			float easeOut(float t, float b, float c, float d) const;
			float easeInOut(float t, float b, float c, float d) const;
		};

		static const Linear  fLinear;
		static const Sine    fSine;
		static const Quint   fQuint;
		static const Quart   fQuart;
		static const Quad    fQuad;
		static const Expo    fExpo;
		static const Elastic fElastic;
		static const Cubic   fCubic;
		static const Circ    fCirc;
		static const Bounce  fBounce;
		static const Back    fBack;

		static const EasingBase *GetFunctionFromEnum(Transition transition)
		{
			switch (transition)
			{
			case LINEAR  : return &fLinear;  break;
			case SINE    : return &fSine;    break;
			case QUINT   : return &fQuint;   break;
			case QUART   : return &fQuart;   break;
			case QUAD    : return &fQuad;    break;
			case EXPO    : return &fExpo;    break;
			case ELASTIC : return &fElastic; break;
			case CUBIC   : return &fCubic;   break;
			case CIRC    : return &fCirc;    break;
			case BOUNCE  : return &fBounce;  break;
			case BACK    : return &fBack;    break;
			default      : return 0;         break;
			}
		}
	}
}

#endif // Easing_h__