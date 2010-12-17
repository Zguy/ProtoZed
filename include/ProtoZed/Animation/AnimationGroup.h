#ifndef AnimationGroup_h__
#define AnimationGroup_h__

#include "Animation.h"

namespace PZ
{
	struct AnimationGroupProperties : public AnimationProperties
	{
	public:
		AnimationGroupProperties(bool async) : async(async)
		{}

		bool async;
	};

	class AnimationGroupImpl;

	class AnimationGroup : public AnimationBase
	{
	public:
		AnimationGroup();
		AnimationGroup(AnimationProperties *properties);
		AnimationGroup(const AnimationGroup &copy);
		~AnimationGroup();

		void SetAsync(bool async);

		void AddAnimation(AnimationBase *animation);

		virtual AnimationBase *GetCopy();

	protected:
		virtual bool StartImpl(AnimablePtr object);

		virtual void AddTime(float deltaTime);

	private:
		AnimationGroupImpl *p;
	};
}

#endif // AnimationGroup_h__