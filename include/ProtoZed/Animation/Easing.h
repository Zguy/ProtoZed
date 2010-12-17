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