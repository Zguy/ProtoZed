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
#include "ProtoZed/Animation/AnimationManager.h"

#include <map>
#include <vector>

#include "ProtoZed/Animation/AnimationBase.h"

namespace PZ
{
	typedef std::map<std::string, AnimationBase*> AnimationMap;
	typedef std::vector<AnimationBase*> AnimationList;

	class AnimationManagerImpl
	{
	public:
		AnimationMap  animationMap;
		AnimationList animations;

		AnimationFactory animationFactory;
	};

	AnimationManager::AnimationManager()
	{
		p = new AnimationManagerImpl;
	}
	AnimationManager::~AnimationManager()
	{
		for (AnimationMap::iterator it = p->animationMap.begin(); it != p->animationMap.end(); ++it)
			delete (*it).second;
		p->animationMap.clear();

		for (AnimationList::iterator it = p->animations.begin(); it != p->animations.end(); ++it)
			delete (*it);
		p->animations.clear();

		delete p;
	}

	bool AnimationManager::UnregisterAnimationType(const std::string &animationType)
	{
		return p->animationFactory.Unregister(animationType);
	}

	AnimationBase *AnimationManager::GetNewAnimation(const std::string &animationType, AnimationProperties *properties)
	{
		return p->animationFactory.Create(animationType, properties);
	}

	bool AnimationManager::HasAnimation(const std::string &animationName)
	{
		return (GetAnimationFromName(animationName) != NULL);
	}

	bool AnimationManager::AddAnimation(const std::string &animationName, AnimationBase *animation)
	{
		if (GetAnimationFromName(animationName) == NULL)
		{
			p->animationMap[animationName] = animation;
			return true;
		}
		else
		{
			return false;
		}
	}
	bool AnimationManager::AddAnimationFromXML(const std::string &animationName, const std::string &filename)
	{
		// Not implemented yet
		return false;
	}
	bool AnimationManager::RemoveAnimation(const std::string &animationName)
	{
		AnimationMap::iterator it = p->animationMap.find(animationName);
		if (it != p->animationMap.end())
		{
			delete (*it).second;
			p->animationMap.erase(it);

			return true;
		}
		else
		{
			return false;
		}
	}

	AnimationBase *AnimationManager::GetAnimationFromName(const std::string &animationName)
	{
		if (p->animationMap.find(animationName) != p->animationMap.end())
			return p->animationMap[animationName];
		else
			return NULL;
	}

	AnimationBase *AnimationManager::RunAnimation(const std::string &animationName, Animable *object)
	{
		AnimationBase *original = GetAnimationFromName(animationName);

		if (original != NULL)
		{
			AnimationBase *copy = original->GetCopy();
			p->animations.push_back(copy);
			copy->Start(object);
			return copy;
		}
		else
		{
			return NULL;
		}
	}

	void AnimationManager::RunAnimationDirect(AnimationBase *animation, Animable *object)
	{
		p->animations.push_back(animation);
		animation->Start(object);
	}

	void AnimationManager::Step(float deltaTime)
	{
		for (AnimationList::iterator it = p->animations.begin(); it != p->animations.end();)
		{
			AnimationBase *animation = (*it);

			if (animation->GetState() == AnimationBase::STARTED)
			{
				if (!animation->onStart.empty())
					animation->onStart();
				animation->state = AnimationBase::RUNNING;
			}

			if (animation->GetState() == AnimationBase::RUNNING)
				animation->AddTime(deltaTime);

			if (animation->GetState() == AnimationBase::FINISHED)
			{
				if (!animation->onFinished.empty())
					animation->onFinished();
				delete animation;
				it = p->animations.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	AnimationFactory &AnimationManager::getAnimationFactory()
	{
		return p->animationFactory;
	}
}