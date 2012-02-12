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
#ifndef PZ_Easing_h__
#define PZ_Easing_h__

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
			EasingBase() {}
			virtual float easeIn(float t, float b, float c, float d) const    = 0;
			virtual float easeOut(float t, float b, float c, float d) const   = 0;
			virtual float easeInOut(float t, float b, float c, float d) const = 0;
		};
		class Back : public EasingBase
		{
		public:
			Back() {}
			virtual float easeIn(float t, float b, float c, float d) const;
			virtual float easeOut(float t, float b, float c, float d) const;
			virtual float easeInOut(float t, float b, float c, float d) const;
		};
		class Bounce : public EasingBase
		{
		public:
			Bounce() {}
			virtual float easeIn(float t, float b, float c, float d) const;
			virtual float easeOut(float t, float b, float c, float d) const;
			virtual float easeInOut(float t, float b, float c, float d) const;
		};
		class Circ : public EasingBase
		{
		public:
			Circ() {}
			float easeIn(float t, float b, float c, float d) const;
			float easeOut(float t, float b, float c, float d) const;
			float easeInOut(float t, float b, float c, float d) const;
		};
		class Cubic : public EasingBase
		{
		public:
			Cubic() {}
			float easeIn(float t, float b, float c, float d) const;
			float easeOut(float t, float b, float c, float d) const;
			float easeInOut(float t, float b, float c, float d) const;
		};
		class Elastic : public EasingBase
		{
		public:
			Elastic() {}
			float easeIn(float t, float b, float c, float d) const;
			float easeOut(float t, float b, float c, float d) const;
			float easeInOut(float t, float b, float c, float d) const;
		};
		class Expo : public EasingBase
		{
		public:
			Expo() {}
			float easeIn(float t, float b, float c, float d) const;
			float easeOut(float t, float b, float c, float d) const;
			float easeInOut(float t, float b, float c, float d) const;
		};
		class Quad : public EasingBase
		{
		public:
			Quad() {}
			float easeIn(float t, float b, float c, float d) const;
			float easeOut(float t, float b, float c, float d) const;
			float easeInOut(float t, float b, float c, float d) const;
		};
		class Quart : public EasingBase
		{
		public:
			Quart() {}
			float easeIn(float t, float b, float c, float d) const;
			float easeOut(float t, float b, float c, float d) const;
			float easeInOut(float t, float b, float c, float d) const;
		};
		class Quint : public EasingBase
		{
		public:
			Quint() {}
			float easeIn(float t, float b, float c, float d) const;
			float easeOut(float t, float b, float c, float d) const;
			float easeInOut(float t, float b, float c, float d) const;
		};
		class Sine : public EasingBase
		{
		public:
			Sine() {}
			float easeIn(float t, float b, float c, float d) const;
			float easeOut(float t, float b, float c, float d) const;
			float easeInOut(float t, float b, float c, float d) const;
		};
		class Linear : public EasingBase
		{
		public:
			Linear() {}
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

#endif // PZ_Easing_h__