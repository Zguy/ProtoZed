/*
Copyright (c) 2013 Johannes Häggqvist

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
#include <ProtoZed/Animation/Animable.h>

#include <ProtoZed/Animation/AnimationBase.h>
#include <ProtoZed/Animation/AnimationManager.h>
#include <ProtoZed/Application.h>

namespace PZ
{
	Animable::Animable(AnimationManager &animationManager) : animationManager(animationManager)
	{
	}
	Animable::Animable(const Animable &other) : animationManager(other.GetAnimationManager())
	{
	}
	Animable::~Animable()
	{
		for (AnimationList::iterator it = animations.begin(); it != animations.end(); ++it)
		{
			GetAnimationManager().DestroyAnimation((*it));
		}
		animations.clear();
	}

	AnimationBase *Animable::RunAnimation(const std::string &animationName)
	{
		AnimationBase *original = GetAnimationManager().GetAnimationFromName(animationName);

		if (original != nullptr)
		{
			AnimationBase *copy = original->GetCopy();
			animations.push_back(copy);
			copy->Start(*this);
			return copy;
		}
		else
		{
			return nullptr;
		}
	}
	AnimationBase *Animable::RunAnimationDirect(AnimationBase *animation)
	{
		if (animation != nullptr)
		{
			animations.push_back(animation);
			animation->Start(*this);
		}

		return animation;
	}
	AnimationBase *Animable::RunAnimationDirect(const std::string &animationType, AnimationProperties *properties)
	{
		return RunAnimationDirect(GetAnimationManager().CreateAnimation(animationType, properties));
	}

	void Animable::StopAllAnimations()
	{
		for (AnimationList::iterator it = animations.begin(); it != animations.end(); ++it)
		{
			(*it)->Stop();
		}
	}

	void Animable::StepAnimations(float deltaTime)
	{
		for (AnimationList::iterator it = animations.begin(); it != animations.end();)
		{
			AnimationBase *animation = (*it);

			if (animation->GetState() == AnimationBase::STARTED)
			{
				/*if (!animation->onStart.empty())
					animation->onStart(animation);*/

				animation->state = AnimationBase::RUNNING;
			}

			if (animation->GetState() == AnimationBase::RUNNING)
				animation->AddTime(deltaTime);

			if (animation->GetState() == AnimationBase::FINISHED)
			{
				/*if (!animation->onFinished.empty())
					animation->onFinished(animation);*/

				GetAnimationManager().DestroyAnimation(animation);
				it = animations.erase(it);
			}
			else if (animation->GetState() == AnimationBase::STOPPED)
			{
				GetAnimationManager().DestroyAnimation(animation);
				it = animations.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}