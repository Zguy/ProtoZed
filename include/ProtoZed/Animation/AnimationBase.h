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
#ifndef AnimationBase_h__
#define AnimationBase_h__

#include <ProtoZed/Animation/Animable.h>

#include <ProtoZed/FastDelegate/delegate.h>

#include <memory>

namespace PZ
{
	struct AnimationProperties
	{
	};

	class AnimationBase;
	typedef fd::delegate1<void, AnimationBase*> AnimationCallback;

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
			STOPPED,
			FINISHED
		};

		AnimationBase();
		AnimationBase(AnimationProperties *properties);
		virtual ~AnimationBase();

		void Start(AnimablePtr animable);
		void Stop();

		void Pause(bool pause = true);
		void Resume();
		inline bool IsPaused() const { return (state == PAUSED); }

		virtual AnimationBase *GetCopy() = 0;

		inline void SetStartCallback(AnimationCallback &callback) { onStart = callback; }
		inline void SetFinishedCallback(AnimationCallback &callback) { onFinished = callback; }

		inline AnimablePtr GetAnimable() { return object.lock(); }

		inline State GetState() const { return state; }

	protected:
		virtual bool StartImpl() = 0;

		virtual void AddTime(float deltaTime) = 0;

		AnimableWeakPtr object;

		State state;

		AnimationCallback onStart;
		AnimationCallback onFinished;
	};
}

#endif // AnimationBase_h__