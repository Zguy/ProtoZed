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
#ifndef Easing_h__
#define Easing_h__

#include <cstdlib>

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

		class Easing
		{
		public:
			Easing()
			{}

			virtual float easeIn(float t, float b, float c, float d) const    = 0;
			virtual float easeOut(float t, float b, float c, float d) const   = 0;
			virtual float easeInOut(float t, float b, float c, float d) const = 0;
		};
		class Back : public Easing
		{
		public:
			virtual float easeIn(float t, float b, float c, float d) const;
			virtual float easeOut(float t, float b, float c, float d) const;
			virtual float easeInOut(float t, float b, float c, float d) const;
		};
		class Bounce : public Easing
		{
		public:
			virtual float easeIn(float t, float b, float c, float d) const;
			virtual float easeOut(float t, float b, float c, float d) const;
			virtual float easeInOut(float t, float b, float c, float d) const;
		};
		class Circ : public Easing
		{
		public:
			float easeIn(float t, float b, float c, float d) const;
			float easeOut(float t, float b, float c, float d) const;
			float easeInOut(float t, float b, float c, float d) const;
		};
		class Cubic : public Easing
		{
		public:
			float easeIn(float t, float b, float c, float d) const;
			float easeOut(float t, float b, float c, float d) const;
			float easeInOut(float t, float b, float c, float d) const;
		};
		class Elastic : public Easing
		{
		public:
			float easeIn(float t, float b, float c, float d) const;
			float easeOut(float t, float b, float c, float d) const;
			float easeInOut(float t, float b, float c, float d) const;
		};
		class Expo : public Easing
		{
		public:
			float easeIn(float t, float b, float c, float d) const;
			float easeOut(float t, float b, float c, float d) const;
			float easeInOut(float t, float b, float c, float d) const;
		};
		class Quad : public Easing
		{
		public:
			float easeIn(float t, float b, float c, float d) const;
			float easeOut(float t, float b, float c, float d) const;
			float easeInOut(float t, float b, float c, float d) const;
		};
		class Quart : public Easing
		{
		public:
			float easeIn(float t, float b, float c, float d) const;
			float easeOut(float t, float b, float c, float d) const;
			float easeInOut(float t, float b, float c, float d) const;
		};
		class Quint : public Easing
		{
		public :
			float easeIn(float t, float b, float c, float d) const;
			float easeOut(float t, float b, float c, float d) const;
			float easeInOut(float t, float b, float c, float d) const;
		};
		class Sine : public Easing
		{
		public :
			float easeIn(float t, float b, float c, float d) const;
			float easeOut(float t, float b, float c, float d) const;
			float easeInOut(float t, float b, float c, float d) const;
		};
		class Linear : public Easing
		{
		public :
			float easeNone(float t, float b, float c, float d) const;
			float easeIn(float t, float b, float c, float d) const;
			float easeOut(float t, float b, float c, float d) const;
			float easeInOut(float t, float b, float c, float d) const;
		};

		static Linear  fLinear;
		static Sine    fSine;
		static Quint   fQuint;
		static Quart   fQuart;
		static Quad    fQuad;
		static Expo    fExpo;
		static Elastic fElastic;
		static Cubic   fCubic;
		static Circ    fCirc;
		static Bounce  fBounce;
		static Back    fBack;

		static Easing *GetFunctionFromEnum(Transition transition)
		{
			Easing *function = NULL;
			switch (transition)
			{
			case LINEAR  : function = &fLinear;  break;
			case SINE    : function = &fSine;    break;
			case QUINT   : function = &fQuint;   break;
			case QUART   : function = &fQuart;   break;
			case QUAD    : function = &fQuad;    break;
			case EXPO    : function = &fExpo;    break;
			case ELASTIC : function = &fElastic; break;
			case CUBIC   : function = &fCubic;   break;
			case CIRC    : function = &fCirc;    break;
			case BOUNCE  : function = &fBounce;  break;
			case BACK    : function = &fBack;    break;
			}
			return function;
		}
	}
}

#endif // Easing_h__