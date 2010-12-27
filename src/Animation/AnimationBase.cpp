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

	void AnimationBase::Start(AnimablePtr animable)
	{
		if (state == NOT_STARTED)
		{
			object = animable;
			if (StartImpl())
				state = STARTED;
			else
				state = STOPPED;
		}
	}
	void AnimationBase::Stop()
	{
		state = STOPPED;
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