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
#ifndef PZ_AnimationBase_h__
#define PZ_AnimationBase_h__

#include <ProtoZed/Animation/Animable.h>

#include <ProtoZed/FastDelegate/delegate.h>

namespace PZ
{
	struct AnimationProperties
	{
	};

	class AnimationBase;
	typedef fd::delegate1<void, AnimationBase*> AnimationCallback;

	class AnimationBase
	{
		friend class Animable;
		friend class AnimationGroup;

	public:
		enum State
		{
			NOT_STARTED,
			STARTED,
			RUNNING,
			PAUSED,
			STOPPED,
			FINISHED
		};

		AnimationBase(AnimationManager &animationManager);
		AnimationBase(AnimationProperties *properties, AnimationManager &animationManager);
		virtual ~AnimationBase();

		void Start(Animable &animable);
		void Stop();

		void Pause(bool pause = true);
		void Resume();
		inline bool IsPaused() const { return (state == PAUSED); }

		virtual AnimationBase *GetCopy() = 0;

		inline void SetStartCallback(AnimationCallback &callback) { onStart = callback; }
		inline void SetFinishedCallback(AnimationCallback &callback) { onFinished = callback; }

		inline Animable &GetAnimable() { return *object; }

		inline State GetState() const { return state; }

		virtual void AddTime(float deltaTime) = 0;

	protected:
		virtual bool StartImpl() = 0;

		inline AnimationManager &GetAnimationManager() const
		{
			return animationManager;
		}

		Animable *object;

		State state;

		AnimationCallback onStart;
		AnimationCallback onFinished;

	private:
		AnimationManager &animationManager;
	};
}

#endif // PZ_AnimationBase_h__