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
#ifndef Tween_h__
#define Tween_h__

#include <ProtoZed/Animation/AnimationBase.h>
#include <ProtoZed/Animation/Easing.h>

namespace PZ
{
	struct TweenProperties : public AnimationProperties
	{
	public:
		TweenProperties(Animable::Attribute attribute, float goal, Easing::Transition transition, Easing::Equation equation, float time, float delay = 0.f, int repeat = 0, bool pingpong = false, bool relativeGoal = false)
			: attribute(attribute), goal(goal), relativeGoal(relativeGoal), transition(transition), equation(equation), time(time), delay(delay), repeat(repeat), pingpong(pingpong)
		{}

		Animable::Attribute attribute;
		float               goal;
		bool                relativeGoal;
		Easing::Transition  transition;
		Easing::Equation    equation;
		float               time;
		float               delay;
		int                 repeat;
		bool                pingpong;
	};

	class TweenImpl;

	class Tween : public AnimationBase
	{
	public:
		Tween();
		Tween(AnimationProperties *properties);
		Tween(const Tween &copy);
		virtual ~Tween();

		void SetAll(Animable::Attribute attribute, float goal, Easing::Transition transition, Easing::Equation equation, float time, float delay = 0.f, int repeat = 0, bool pingpong = false, bool relativeGoal = false);

		void SetAttribute(Animable::Attribute attribute);
		void SetGoal(float goal);
		void SetRelativeGoal(bool relative);
		void SetTransition(Easing::Transition transition);
		void SetEquation(Easing::Equation equation);
		void SetTime(float time);
		void SetDelay(float delay);
		void SetRepeat(int repeat);
		void SetPingPoing(bool pingpong);

		Animable::Attribute GetAttribute()    const;
		float               GetGoal()         const;
		bool                GetRelativeGoal() const;
		Easing::Transition  GetTransition()   const;
		Easing::Equation    GetEquation()     const;
		float               GetTime()         const;
		float               GetDelay()        const;
		int                 GetRepeat()       const;
		bool                GetPingPong()     const;

		virtual AnimationBase *GetCopy();

	protected:
		virtual bool StartImpl();

		virtual void AddTime(float deltaTime);

	private:
		TweenImpl *p;
	};
}

#endif // Tween_h__