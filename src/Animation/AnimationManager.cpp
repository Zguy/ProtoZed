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
#include <ProtoZed/Animation/AnimationManager.h>

#include <ProtoZed/Animation/AnimationBase.h>

#include <map>
#include <vector>

namespace PZ
{
	typedef std::map<std::string, AnimationBase*> AnimationMap;

	class AnimationManagerImpl
	{
	public:
		AnimationMap  animationMap;

		AnimationManager::AnimationFactory animationFactory;
	};

	AnimationManager::AnimationManager() : p(new AnimationManagerImpl)
	{
	}
	AnimationManager::~AnimationManager()
	{
		for (AnimationMap::iterator it = p->animationMap.begin(); it != p->animationMap.end(); ++it)
			delete (*it).second;
		p->animationMap.clear();

		delete p;
	}

	bool AnimationManager::UnregisterAnimationType(const std::string &animationType)
	{
		return p->animationFactory.Unregister(animationType);
	}

	AnimationBase *AnimationManager::GetNewAnimation(const std::string &animationType, AnimationProperties *properties) const
	{
		return p->animationFactory.Create(animationType, properties);
	}

	void AnimationManager::DestroyAnimation(AnimationBase *animation) const
	{
		delete animation;
	}

	bool AnimationManager::HasAnimation(const std::string &animationName) const
	{
		return (GetAnimationFromName(animationName) != NULL);
	}

	bool AnimationManager::AddAnimation(const std::string &animationName, AnimationBase *animation)
	{
		if (!HasAnimation(animationName))
		{
			p->animationMap.insert(std::make_pair(animationName, animation));
			return true;
		}
		else
		{
			return false;
		}
	}
	bool AnimationManager::AddAnimationFromFile(const std::string &animationName, const std::string &filename)
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

	AnimationBase *AnimationManager::GetAnimationFromName(const std::string &animationName) const
	{
		if (p->animationMap.find(animationName) != p->animationMap.end())
			return p->animationMap.at(animationName);
		else
			return NULL;
	}

	AnimationManager::AnimationFactory &AnimationManager::getAnimationFactory() const
	{
		return p->animationFactory;
	}
}