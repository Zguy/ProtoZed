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
#ifndef PZ_Tween_h__
#define PZ_Tween_h__

#include <ProtoZed/Animation/AnimationBase.h>
#include <ProtoZed/Animation/Easing.h>

namespace PZ
{
	struct TweenProperties : public AnimationProperties
	{
	public:
		TweenProperties(Attribute attribute, float goal, Easing::Transition transition, Easing::Equation equation, float time, float delay = 0.f, int repeat = 0, bool pingpong = false, bool relativeGoal = false)
			: attribute(attribute), goal(goal), relativeGoal(relativeGoal), transition(transition), equation(equation), time(time), delay(delay), repeat(repeat), pingpong(pingpong)
		{}

		Attribute           attribute;
		float               goal;
		bool                relativeGoal;
		Easing::Transition  transition;
		Easing::Equation    equation;
		float               time;
		float               delay;
		int                 repeat;
		bool                pingpong;
	};

	class Tween : public AnimationBase
	{
	public:
		Tween(AnimationManager &animationManager);
		Tween(AnimationProperties *properties, AnimationManager &animationManager);
		Tween(const Tween &copy);
		~Tween();

		void SetAll(Attribute attribute, float goal, Easing::Transition transition, Easing::Equation equation, float time, float delay = 0.f, int repeat = 0, bool pingpong = false, bool relativeGoal = false);

		void SetAttribute(Attribute attribute);
		void SetGoal(float goal);
		void SetRelativeGoal(bool relative);
		void SetTransition(Easing::Transition transition);
		void SetEquation(Easing::Equation equation);
		void SetTime(float time);
		void SetDelay(float delay);
		void SetRepeat(int repeat);
		void SetPingPoing(bool pingpong);

		Attribute           GetAttribute()    const;
		float               GetGoal()         const;
		bool                GetRelativeGoal() const;
		Easing::Transition  GetTransition()   const;
		Easing::Equation    GetEquation()     const;
		float               GetTime()         const;
		float               GetDelay()        const;
		int                 GetRepeat()       const;
		bool                GetPingPong()     const;

		virtual AnimationBase *GetCopy();

		virtual void AddTime(float deltaTime);

	protected:
		virtual bool StartImpl();

	private:
		class Impl;
		Impl *p;
	};
}

#endif // PZ_Tween_h__
