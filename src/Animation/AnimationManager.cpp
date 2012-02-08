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
#include <ProtoZed/Animation/AnimationManager.h>

#include <ProtoZed/Animation/AnimationBase.h>

#include <map>
#include <vector>

namespace PZ
{
	typedef std::map<std::string, AnimationBase*> AnimationMap;

	class AnimationManager::Impl
	{
	public:
		AnimationMap  animationMap;

		AnimationManager::AnimationFactory animationFactory;
	};

	AnimationManager::AnimationManager() : p(new Impl)
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

	AnimationBase *AnimationManager::CreateAnimation(const std::string &animationType, AnimationProperties *properties) const
	{
		return p->animationFactory.Create(animationType, properties, const_cast<AnimationManager&>(*this));
	}

	void AnimationManager::DestroyAnimation(AnimationBase *animation) const
	{
		delete animation;
	}

	bool AnimationManager::HasAnimation(const std::string &animationName) const
	{
		return (GetAnimationFromName(animationName) != nullptr);
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
	bool AnimationManager::AddAnimation(const std::string &animationName, const std::string &animationType, AnimationProperties *properties)
	{
		return AddAnimation(animationName, CreateAnimation(animationType, properties));
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
			return nullptr;
	}

	AnimationManager::AnimationFactory &AnimationManager::getAnimationFactory() const
	{
		return p->animationFactory;
	}
}