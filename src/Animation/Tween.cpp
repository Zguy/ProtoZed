/*
Copyright (c) 2013 Johannes H�ggqvist

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
#include <ProtoZed/Animation/Tween.h>

#include <cstdlib>

namespace PZ
{
	class Tween::Impl
	{
	public:
		Impl() : startValue(0.f), totalTime(0.f), currentTime(0.f), attribute(0), goal(1.f), relativeGoal(false), transitionEnum(Easing::LINEAR), transition(&Easing::fLinear), equation(Easing::EASE_IN_OUT), delay(0.f), repeat(0), pingpong(false)
		{}

		float startValue;

		float totalTime;
		float currentTime;

		Attribute               attribute;
		float                   goal;
		bool                    relativeGoal;
		Easing::Transition      transitionEnum;
		const Easing::EasingBase *transition;
		Easing::Equation        equation;
		float                   delay;
		int                     repeat;
		bool                    pingpong;
	};

	Tween::Tween(AnimationManager &animationManager) : AnimationBase(animationManager), p(new Impl)
	{
	}
	Tween::Tween(AnimationProperties *properties, AnimationManager &animationManager) : AnimationBase(properties, animationManager), p(new Impl)
	{
		if (properties != nullptr)
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
	Tween::Tween(const Tween &copy) : AnimationBase(copy.GetAnimationManager()), p(new Impl)
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
						{
							std::swap(p->startValue, p->goal);
						}
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