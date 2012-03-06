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
#ifndef PZ_AnimationManager_h__
#define PZ_AnimationManager_h__

#include <ProtoZed/NonCopyable.h>
#include <ProtoZed/Animation/Animable.h>
#include <ProtoZed/ObjectFactory/ObjectFactory.h>

#include <string>

namespace PZ
{
	class AnimationBase;
	struct AnimationProperties;

	class AnimationManager : public NonCopyable
	{
	public:
		AnimationManager();
		~AnimationManager();

		template<class T>
		bool RegisterAnimationType(const std::string &animationType)
		{
			return getAnimationFactory().Register<T>(animationType);
		}
		bool UnregisterAnimationType(const std::string &animationType);

		AnimationBase *CreateAnimation(const std::string &animationType, AnimationProperties *properties = nullptr) const;
		template<class T>
		T *CreateAnimation(const std::string &animationType, AnimationProperties *properties = nullptr) const
		{
			return static_cast<T*>(CreateAnimation(animationType, properties));
		}

		void DestroyAnimation(AnimationBase *animation) const;

		bool HasAnimation(const std::string &animationName) const;
		
		bool AddAnimation(const std::string &animationName, AnimationBase *animation);
		bool AddAnimation(const std::string &animationName, const std::string &animationType, AnimationProperties *properties);
		bool AddAnimationFromFile(const std::string &animationName, const std::string &filename);
		bool RemoveAnimation(const std::string &animationName);

		AnimationBase *GetAnimationFromName(const std::string &animationName) const;

	private:
		class Impl;
		Impl *p;

		typedef ::ObjectFactory<AnimationBase*(AnimationProperties*, AnimationManager&), std::string> AnimationFactory;
		AnimationFactory &getAnimationFactory() const;
	};
}

#endif // PZ_AnimationManager_h__