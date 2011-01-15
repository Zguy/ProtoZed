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
#include <ProtoZed/Animation/AnimationGroup.h>

#include <list>

namespace PZ
{
	typedef std::list<AnimationBase*> AnimationQueue;

	class AnimationGroupImpl
	{
	public:
		AnimationGroupImpl() : async(false)
		{}

		bool async;

		AnimationQueue animations;
	};

	AnimationGroup::AnimationGroup() : p(new AnimationGroupImpl)
	{
	}
	AnimationGroup::AnimationGroup(AnimationProperties *properties) : p(new AnimationGroupImpl)
	{
		if (properties != NULL)
		{
			AnimationGroupProperties *props = static_cast<AnimationGroupProperties*>(properties);
			p->async = props->async;
		}
	}
	AnimationGroup::AnimationGroup(const AnimationGroup &copy) : p(new AnimationGroupImpl)
	{
		p->async = copy.p->async;
		for (AnimationQueue::iterator it = copy.p->animations.begin(); it != copy.p->animations.end(); ++it)
			p->animations.push_back((*it)->GetCopy());
	}
	AnimationGroup::~AnimationGroup()
	{
		for (AnimationQueue::iterator it = p->animations.begin(); it != p->animations.end(); ++it)
			delete (*it);
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

	AnimationBase *AnimationGroup::GetCopy()
	{
		return new AnimationGroup(*this);
	}

	bool AnimationGroup::StartImpl()
	{
		AnimablePtr animable = object.lock();
		if (p->async)
		{
			for (AnimationQueue::iterator it = p->animations.begin(); it != p->animations.end(); ++it)
			{
				(*it)->Start(animable);
			}
		}
		else
		{
			p->animations.front()->Start(animable);
		}

		return true;
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

					if (animation->GetState() == FINISHED)
					{
						delete (*it);
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

				if (animation->GetState() == FINISHED)
				{
					delete p->animations.front();
					p->animations.pop_front();

					if (!p->animations.empty())
						p->animations.front()->Start(object.lock());
				}

				if (p->animations.empty())
				{
					state = FINISHED;
				}
			}
		}
	}
}