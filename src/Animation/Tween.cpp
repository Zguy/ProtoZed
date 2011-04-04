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
#include <ProtoZed/Animation/Tween.h>

#include <cstdlib>

namespace PZ
{
	class TweenImpl
	{
	public:
		TweenImpl() : startValue(0.f), totalTime(0.f), currentTime(0.f), attribute(""), goal(1.f), relativeGoal(false), transitionEnum(Easing::LINEAR), transition(&Easing::fLinear), equation(Easing::EASE_IN_OUT), delay(0.f), repeat(0), pingpong(false)
		{}

		float startValue;

		float totalTime;
		float currentTime;

		Attribute            attribute;
		float                goal;
		bool                 relativeGoal;
		Easing::Transition   transitionEnum;
		const Easing::Easing *transition;
		Easing::Equation     equation;
		float                delay;
		int                  repeat;
		bool                 pingpong;
	};

	Tween::Tween() : p(new TweenImpl)
	{
	}
	Tween::Tween(AnimationProperties *properties) : p(new TweenImpl)
	{
		if (properties != NULL)
		{
			TweenProperties *props = static_cast<TweenProperties*>(properties);
			p->attribute      = props->attribute;
			p->goal           = props->goal;
			p->relativeGoal   = props->relativeGoal;
			p->transitionEnum = props->transition;
			p->transition     = Easing::GetFunctionFromEnum(props->transition);
			p->equation       = props->equation;
			p->totalTime      = props->time;
			p->delay          = props->delay;
			p->repeat         = props->repeat;
			p->pingpong       = props->pingpong;
		}
	}
	Tween::Tween(const Tween &copy) : p(new TweenImpl)
	{
		p->attribute      = copy.p->attribute;
		p->goal           = copy.p->goal;
		p->relativeGoal   = copy.p->relativeGoal;
		p->transitionEnum = copy.p->transitionEnum;
		p->transition     = copy.p->transition;
		p->equation       = copy.p->equation;
		p->totalTime      = copy.p->totalTime;
		p->delay          = copy.p->delay;
		p->repeat         = copy.p->repeat;
		p->pingpong       = copy.p->pingpong;
	}
	Tween::~Tween()
	{
		delete p;
	}

	void Tween::SetAll(Attribute attribute, float goal, Easing::Transition transition, Easing::Equation equation, float time, float delay, int repeat, bool pingpong, bool relativeGoal)
	{
		p->attribute      = attribute;
		p->goal           = goal;
		p->relativeGoal   = relativeGoal;
		p->transitionEnum = transition;
		p->transition     = Easing::GetFunctionFromEnum(transition);
		p->equation       = equation;
		p->totalTime      = time;
		p->delay          = delay;
		p->repeat         = repeat;
		p->pingpong       = pingpong;
	}

	void Tween::SetAttribute(Attribute attribute)
	{
		p->attribute = attribute;
	}
	void Tween::SetGoal(float goal)
	{
		p->goal = goal;
	}
	void Tween::SetRelativeGoal(bool relative)
	{
		p->relativeGoal = relative;
	}
	void Tween::SetTransition(Easing::Transition transition)
	{
		p->transitionEnum = transition;
		p->transition = Easing::GetFunctionFromEnum(transition);
	}
	void Tween::SetEquation(Easing::Equation equation)
	{
		p->equation = equation;
	}
	void Tween::SetTime(float time)
	{
		p->totalTime = time;
	}
	void Tween::SetDelay(float delay)
	{
		p->delay = delay;
	}
	void Tween::SetRepeat(int repeat)
	{
		p->repeat = repeat;
	}
	void Tween::SetPingPoing(bool pingpong)
	{
		p->pingpong = pingpong;
	}

	Attribute Tween::GetAttribute() const
	{
		return p->attribute;
	}
	float Tween::GetGoal() const
	{
		return p->goal;
	}
	bool Tween::GetRelativeGoal() const
	{
		return p->relativeGoal;
	}
	Easing::Transition Tween::GetTransition() const
	{
		return p->transitionEnum;
	}
	Easing::Equation Tween::GetEquation() const
	{
		return p->equation;
	}
	float Tween::GetTime() const
	{
		return p->totalTime;
	}
	float Tween::GetDelay() const
	{
		return p->delay;
	}
	int Tween::GetRepeat() const
	{
		return p->repeat;
	}
	bool Tween::GetPingPong() const
	{
		return p->pingpong;
	}

	AnimationBase *Tween::GetCopy()
	{
		return new Tween(*this);
	}

	void Tween::AddTime(float deltaTime)
	{
		if (state == RUNNING)
		{
			p->currentTime += deltaTime;

			float goal = (p->relativeGoal ? (p->startValue + p->goal) : p->goal);

			if (p->currentTime > 0.f)
			{
				float newValue = object->GetAttribute(p->attribute);

				switch (p->equation)
				{
				case Easing::EASE_IN     : newValue = p->transition->easeIn(p->currentTime, p->startValue, (goal - p->startValue), p->totalTime);    break;
				case Easing::EASE_OUT    : newValue = p->transition->easeOut(p->currentTime, p->startValue, (goal - p->startValue), p->totalTime);   break;
				case Easing::EASE_IN_OUT : newValue = p->transition->easeInOut(p->currentTime, p->startValue, (goal - p->startValue), p->totalTime); break;
				}

				object->SetAttribute(p->attribute, newValue);
			}
			if (p->currentTime >= p->totalTime)
			{
				if ((p->repeat != -1)&&(--p->repeat <= 0))
				{
					state = FINISHED;
					object->SetAttribute(p->attribute, goal);
				}
				else
				{
					p->currentTime = 0.f;

					if (p->pingpong)
					{
						if (p->relativeGoal)
						{
							p->goal = p->startValue - goal;
							p->startValue = goal;
						}
						else
							std::swap(p->startValue, p->goal);
					}
				}
			}
		}
	}

	bool Tween::StartImpl()
	{
		if (object->HasAttribute(p->attribute))
		{
			p->startValue  = object->GetAttribute(p->attribute);
			p->currentTime = -p->delay;
		}
		else
		{
			state = STOPPED;
		}

		return true;
	}
}