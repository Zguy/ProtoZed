#include "ProtoZed/Animation/AnimationBase.h"

namespace PZ
{
	AnimationBase::AnimationBase() : state(NOT_STARTED)
	{

	}
	AnimationBase::AnimationBase(AnimationProperties *properties) : state(NOT_STARTED)
	{

	}
	AnimationBase::~AnimationBase()
	{

	}

	void AnimationBase::Start(AnimablePtr object)
	{
		if (state == NOT_STARTED)
		{
			if (StartImpl(object))
				state = STARTED;
			else
				state = FINISHED;
		}
	}
	void AnimationBase::Stop()
	{
		state = FINISHED;
	}

	void AnimationBase::Pause(bool pause)
	{
		if (pause)
		{
			if (state == RUNNING)
			{
				state = PAUSED;
			}
		}
		else
		{
			if (state == PAUSED)
			{
				state = RUNNING;
			}
		}
	}
	void AnimationBase::Resume()
	{
		Pause(false);
	}
}