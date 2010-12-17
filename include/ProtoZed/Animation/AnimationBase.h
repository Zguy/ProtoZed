#ifndef AnimationBase_h__
#define AnimationBase_h__

#include <memory>

#include <delegate.h>

#include "Animable.h"

namespace PZ
{
	struct AnimationProperties
	{
	};

	class AnimationBase;
	typedef fd::delegate0<void> AnimationCallback;

	class AnimationBase
	{
		friend class AnimationManager;
		friend class AnimationGroup;

	public:
		enum State
		{
			NOT_STARTED,
			STARTED,
			RUNNING,
			PAUSED,
			FINISHED
		};

		AnimationBase();
		AnimationBase(AnimationProperties *properties);
		virtual ~AnimationBase();

		void Start(AnimablePtr object);
		void Stop();

		void Pause(bool pause = true);
		void Resume();
		inline bool IsPaused() { return (state == PAUSED); }

		virtual AnimationBase *GetCopy() = 0;

		inline void SetStartCallback(AnimationCallback &callback) { onStart = callback; }
		inline void SetFinishedCallback(AnimationCallback &callback) { onFinished = callback; }

		inline State GetState() { return state; }

	protected:
		virtual bool StartImpl(AnimablePtr object) = 0;

		virtual void AddTime(float deltaTime) = 0;

		State state;

		AnimationCallback onStart;
		AnimationCallback onFinished;
	};
}

#endif // AnimationBase_h__