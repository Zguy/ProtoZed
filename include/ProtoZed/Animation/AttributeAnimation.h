#ifndef AttributeAnimation_h__
#define AttributeAnimation_h__

#include "Animation.h"
#include "Easing.h"

namespace PZ
{
	struct AttributeAnimationProperties : public AnimationProperties
	{
	public:
		AttributeAnimationProperties(Animable::Attribute attribute, float goal, Easing::Transition transition, Easing::Equation equation, float time, float delay = 0.f, int repeat = 0, bool pingpong = false, bool relativeGoal = false)
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

	class AttributeAnimationImpl;

	class AttributeAnimation : public AnimationBase
	{
	public:
		AttributeAnimation();
		AttributeAnimation(AnimationProperties *properties);
		AttributeAnimation(const AttributeAnimation &copy);
		virtual ~AttributeAnimation();

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

		Animable::Attribute GetAttribute();
		float               GetGoal();
		bool                GetRelativeGoal();
		Easing::Transition  GetTransition();
		Easing::Equation    GetEquation();
		float               GetTime();
		float               GetDelay();
		int                 GetRepeat();
		bool                GetPingPong();

		virtual AnimationBase *GetCopy();

	protected:
		virtual bool StartImpl(AnimablePtr object);

		virtual void AddTime(float deltaTime);

	private:
		AttributeAnimationImpl *p;
	};
}

#endif // AttributeAnimation_h__