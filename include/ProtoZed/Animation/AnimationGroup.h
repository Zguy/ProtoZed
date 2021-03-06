/*
Copyright (c) 2013 Johannes H�ggqvist

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
#ifndef PZ_AnimationGroup_h__
#define PZ_AnimationGroup_h__

#include <ProtoZed/Animation/AnimationBase.h>

namespace PZ
{
	struct AnimationGroupProperties : public AnimationProperties
	{
	public:
		AnimationGroupProperties(bool async) : async(async)
		{}

		bool async;
	};

	class AnimationGroup : public AnimationBase
	{
	public:
		AnimationGroup(AnimationManager &animationManager);
		AnimationGroup(AnimationProperties *properties, AnimationManager &animationManager);
		AnimationGroup(const AnimationGroup &copy);
		~AnimationGroup();

		void SetAsync(bool async);

		void AddAnimation(AnimationBase *animation);
		void AddAnimation(const std::string &animationName);
		void AddAnimation(const std::string &animationType, AnimationProperties *properties);

		virtual AnimationBase *GetCopy();

		virtual void AddTime(float deltaTime);

	protected:
		virtual bool StartImpl();

	private:
		class Impl;
		Impl *p;
	};
}

#endif // PZ_AnimationGroup_h__
