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
#include <ProtoZed/Animation/AnimationGroup.h>

#include <ProtoZed/Animation/AnimationManager.h>
#include <ProtoZed/Application.h>

#include <list>

namespace PZ
{
	typedef std::list<AnimationBase*> AnimationQueue;

	class AnimationGroup::Impl
	{
	public:
		Impl() : async(false)
		{}

		bool async;

		AnimationQueue animations;
	};

	AnimationGroup::AnimationGroup(AnimationManager &animationManager) : AnimationBase(animationManager), p(new Impl)
	{
	}
	AnimationGroup::AnimationGroup(AnimationProperties *properties, AnimationManager &animationManager) : AnimationBase(properties, animationManager), p(new Impl)
	{
		if (properties != nullptr)
		{
			AnimationGroupProperties *props = static_cast<AnimationGroupProperties*>(properties);
			p->async = props->async;
		}
	}
	AnimationGroup::AnimationGroup(const AnimationGroup &copy) : AnimationBase(copy.GetAnimationManager()), p(new Impl)
	{
		p->async = copy.p->async;
		for (AnimationQueue::iterator it = copy.p->animations.begin(); it != copy.p->animations.end(); ++it)
			p->animations.push_back((*it)->GetCopy());
	}
	AnimationGroup::~AnimationGroup()
	{
		for (AnimationQueue::iterator it = p->animations.begin(); it != p->animations.end(); ++it)
			GetAnimationManager().DestroyAnimation(*it);
		p->animations.clear();

		delete p;
	}

	void AnimationGroup::SetAsync(bool async)
	{
		p->async = async;
	}

	void AnimationGroup::AddAnimation(AnimationBase *animation)
	{
		if (state == NOT_STARTED)
		{
			p->animations.push_back(animation);
		}
	}
	void AnimationGroup::AddAnimation(const std::string &animationName)
	{
		AddAnimation(GetAnimationManager().GetAnimationFromName(animationName)->GetCopy());
	}
	void AnimationGroup::AddAnimation(const std::string &animationType, AnimationProperties *properties)
	{
		AddAnimation(GetAnimationManager().CreateAnimation(animationType, properties));
	}

	AnimationBase *AnimationGroup::GetCopy()
	{
		return new AnimationGroup(*this);
	}

	void AnimationGroup::AddTime(float deltaTime)
	{
		if (state == RUNNING)
		{
			if (p->async)
			{
				for (AnimationQueue::iterator it = p->animations.begin(); it != p->animations.end();)
				{
					AnimationBase *animation = *it;

					if (animation->GetState() == STARTED)
						animation->state = RUNNING;

					if (animation->GetState() == RUNNING)
						animation->AddTime(deltaTime);

					if ((animation->GetState() == FINISHED)||(animation->GetState() == STOPPED))
					{
						GetAnimationManager().DestroyAnimation(animation);
						it = p->animations.erase(it);
					}
					else
					{
						++it;
					}

					if (p->animations.empty())
					{
						state = FINISHED;
					}
				}
			}
			else
			{
				AnimationBase *animation = p->animations.front();

				if (animation->GetState() == STARTED)
					animation->state = RUNNING;

				if (animation->GetState() == RUNNING)
					animation->AddTime(deltaTime);

				if ((animation->GetState() == FINISHED)||(animation->GetState() == STOPPED))
				{
					GetAnimationManager().DestroyAnimation(animation);
					p->animations.pop_front();

					if (!p->animations.empty())
						p->animations.front()->Start(*object);
				}

				if (p->animations.empty())
				{
					state = FINISHED;
				}
			}
		}
	}

	bool AnimationGroup::StartImpl()
	{
		if (p->async)
		{
			for (AnimationQueue::iterator it = p->animations.begin(); it != p->animations.end(); ++it)
			{
				(*it)->Start(*object);
			}
		}
		else
		{
			p->animations.front()->Start(*object);
		}

		return true;
	}
}