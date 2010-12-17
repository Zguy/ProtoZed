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
#include "ProtoZed/Animation/AttributeAnimation.h"

#include <cstdlib>

namespace PZ
{
	class AttributeAnimationImpl
	{
	public:
		AttributeAnimationImpl() : object(NULL), startValue(0.f), totalTime(0.f), currentTime(0.f), attribute(0), goal(1.f), relativeGoal(false), transitionEnum(Easing::LINEAR), transition(&Easing::fLinear), equation(Easing::EASE_IN_OUT), delay(0.f), repeat(0), pingpong(false)
		{}

		AnimablePtr object;
		float startValue;

		float totalTime;
		float currentTime;

		Animable::Attribute attribute;
		float               goal;
		bool                relativeGoal;
		Easing::Transition  transitionEnum;
		Easing::Easing     *transition;
		Easing::Equation    equation;
		float               delay;
		int                 repeat;
		bool                pingpong;
	};

	AttributeAnimation::AttributeAnimation()
	{
		p = new AttributeAnimationImpl;
	}
	AttributeAnimation::AttributeAnimation(AnimationProperties *properties)
	{
		p = new AttributeAnimationImpl;

		if (properties != NULL)
		{
			AttributeAnimationProperties *props = static_cast<AttributeAnimationProperties*>(properties);
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
	AttributeAnimation::AttributeAnimation(const AttributeAnimation &copy)
	{
		p = new AttributeAnimationImpl;

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
	AttributeAnimation::~AttributeAnimation()
	{
		delete p;
	}

	void AttributeAnimation::SetAll(Animable::Attribute attribute, float goal, Easing::Transition transition, Easing::Equation equation, float time, float delay, int repeat, bool pingpong, bool relativeGoal)
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

	void AttributeAnimation::SetAttribute(Animable::Attribute attribute)
	{
		p->attribute = attribute;
	}
	void AttributeAnimation::SetGoal(float goal)
	{
		p->goal = goal;
	}
	void AttributeAnimation::SetRelativeGoal(bool relative)
	{
		p->relativeGoal = relative;
	}
	void AttributeAnimation::SetTransition(Easing::Transition transition)
	{
		p->transitionEnum = transition;
		p->transition = Easing::GetFunctionFromEnum(transition);
	}
	void AttributeAnimation::SetEquation(Easing::Equation equation)
	{
		p->equation = equation;
	}
	void AttributeAnimation::SetTime(float time)
	{
		p->totalTime = time;
	}
	void AttributeAnimation::SetDelay(float delay)
	{
		p->delay = delay;
	}
	void AttributeAnimation::SetRepeat(int repeat)
	{
		p->repeat = repeat;
	}
	void AttributeAnimation::SetPingPoing(bool pingpong)
	{
		p->pingpong = pingpong;
	}

	Animable::Attribute AttributeAnimation::GetAttribute()
	{
		return p->attribute;
	}
	float AttributeAnimation::GetGoal()
	{
		return p->goal;
	}
	bool AttributeAnimation::GetRelativeGoal()
	{
		return p->relativeGoal;
	}
	Easing::Transition AttributeAnimation::GetTransition()
	{
		return p->transitionEnum;
	}
	Easing::Equation AttributeAnimation::GetEquation()
	{
		return p->equation;
	}
	float AttributeAnimation::GetTime()
	{
		return p->totalTime;
	}
	float AttributeAnimation::GetDelay()
	{
		return p->delay;
	}
	int AttributeAnimation::GetRepeat()
	{
		return p->repeat;
	}
	bool AttributeAnimation::GetPingPong()
	{
		return p->pingpong;
	}

	AnimationBase *AttributeAnimation::GetCopy()
	{
		return new AttributeAnimation(*this);
	}

	bool AttributeAnimation::StartImpl(AnimablePtr object)
	{
		if (object->SupportAttribute(p->attribute))
		{
			p->object      = object;
			p->startValue  = object->GetAttribute(p->attribute);
			p->currentTime = -p->delay;

			return true;
		}
		else
		{
			return false;
		}
	}

	void AttributeAnimation::AddTime(float deltaTime)
	{
		if (state == RUNNING)
		{
			p->currentTime += deltaTime;

			float goal = (p->relativeGoal ? (p->startValue + p->goal) : p->goal);

			if (p->currentTime > 0.f)
			{
				float newValue = p->object->GetAttribute(p->attribute);

				switch (p->equation)
				{
				case Easing::EASE_IN     : newValue = p->transition->easeIn(p->currentTime, p->startValue, (goal - p->startValue), p->totalTime);    break;
				case Easing::EASE_OUT    : newValue = p->transition->easeOut(p->currentTime, p->startValue, (goal - p->startValue), p->totalTime);   break;
				case Easing::EASE_IN_OUT : newValue = p->transition->easeInOut(p->currentTime, p->startValue, (goal - p->startValue), p->totalTime); break;
				}

				p->object->SetAttribute(p->attribute, newValue);
			}
			if (p->currentTime >= p->totalTime)
			{
				if ((p->repeat != -1)&&(--p->repeat <= 0))
				{
					state     = FINISHED;
					p->object->SetAttribute(p->attribute, goal);
					p->object = NULL;
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
		else if (state == FINISHED)
		{
			p->object = NULL;
		}
	}
}