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

			virtual float easeIn(float t, float b, float c, float d)    = 0;
			virtual float easeOut(float t, float b, float c, float d)   = 0;
			virtual float easeInOut(float t, float b, float c, float d) = 0;
		};
		class Back : public Easing
		{
		public:
			virtual float easeIn(float t, float b, float c, float d);
			virtual float easeOut(float t, float b, float c, float d);
			virtual float easeInOut(float t, float b, float c, float d);
		};
		class Bounce : public Easing
		{
		public:
			virtual float easeIn(float t, float b, float c, float d);
			virtual float easeOut(float t, float b, float c, float d);
			virtual float easeInOut(float t, float b, float c, float d);
		};
		class Circ : public Easing
		{
		public:
			float easeIn(float t, float b, float c, float d);
			float easeOut(float t, float b, float c, float d);
			float easeInOut(float t, float b, float c, float d);
		};
		class Cubic : public Easing
		{
		public:
			float easeIn(float t, float b, float c, float d);
			float easeOut(float t, float b, float c, float d);
			float easeInOut(float t, float b, float c, float d);
		};
		class Elastic : public Easing
		{
		public:
			float easeIn(float t, float b, float c, float d);
			float easeOut(float t, float b, float c, float d);
			float easeInOut(float t, float b, float c, float d);
		};
		class Expo : public Easing
		{
		public:
			float easeIn(float t, float b, float c, float d);
			float easeOut(float t, float b, float c, float d);
			float easeInOut(float t, float b, float c, float d);
		};
		class Quad : public Easing
		{
		public:
			float easeIn(float t, float b, float c, float d);
			float easeOut(float t, float b, float c, float d);
			float easeInOut(float t, float b, float c, float d);
		};
		class Quart : public Easing
		{
		public:
			float easeIn(float t, float b, float c, float d);
			float easeOut(float t, float b, float c, float d);
			float easeInOut(float t, float b, float c, float d);
		};
		class Quint : public Easing
		{
		public :
			float easeIn(float t, float b, float c, float d);
			float easeOut(float t, float b, float c, float d);
			float easeInOut(float t, float b, float c, float d);
		};
		class Sine : public Easing
		{
		public :
			float easeIn(float t, float b, float c, float d);
			float easeOut(float t, float b, float c, float d);
			float easeInOut(float t, float b, float c, float d);
		};
		class Linear : public Easing
		{
		public :
			float easeNone(float t, float b, float c, float d);
			float easeIn(float t, float b, float c, float d);
			float easeOut(float t, float b, float c, float d);
			float easeInOut(float t, float b, float c, float d);
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

		Easing *GetFunctionFromEnum(Transition transition);
	}
}

#endif // Easing_h__