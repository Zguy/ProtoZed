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
#include <ProtoZed/Animation/Animable.h>

#include <ProtoZed/Animation/AnimationBase.h>
#include <ProtoZed/Animation/AnimationManager.h>
#include <ProtoZed/Application.h>

namespace PZ
{
	Animable::Animable()
	{
	}
	Animable::~Animable()
	{
		for (AnimationList::iterator it = animations.begin(); it != animations.end(); ++it)
		{
			(*it)->Stop();
			delete (*it);
		}
	}

	AnimationBase *Animable::RunAnimation(const std::string &animationName)
	{
		AnimationManager &animationManager = PZ::Application::GetSingleton().GetAnimationManager();
		AnimationBase *original = animationManager.GetAnimationFromName(animationName);

		if (original != NULL)
		{
			AnimationBase *copy = original->GetCopy();
			animations.push_back(copy);
			copy->Start(*this);
			return copy;
		}
		else
		{
			return NULL;
		}
	}
	AnimationBase *Animable::RunAnimationDirect(AnimationBase *animation)
	{
		if (animation != NULL)
		{
			animations.push_back(animation);
			animation->Start(*this);
		}

		return animation;
	}

	void Animable::StepAnimations(float deltaTime)
	{
		for (AnimationList::iterator it = animations.begin(); it != animations.end();)
		{
			AnimationBase *animation = (*it);

			if (animation->GetState() == AnimationBase::STARTED)
			{
				if (!animation->onStart.empty())
					animation->onStart(animation);

				animation->state = AnimationBase::RUNNING;
			}

			if (animation->GetState() == AnimationBase::RUNNING)
				animation->AddTime(deltaTime);

			if (animation->GetState() == AnimationBase::FINISHED)
			{
				if (!animation->onFinished.empty())
					animation->onFinished(animation);

				delete animation;
				it = animations.erase(it);
			}
			else if (animation->GetState() == AnimationBase::STOPPED)
			{
				delete animation;
				it = animations.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}