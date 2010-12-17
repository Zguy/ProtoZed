#ifndef AnimationManager_h__
#define AnimationManager_h__

#include <string>

#include "Animable.h"

#include "ProtoZed/ObjectFactory/ObjectFactory.h"

namespace PZ
{
	class AnimationBase;
	struct AnimationProperties;

	typedef ::ObjectFactory<AnimationBase*(AnimationProperties*), std::string> AnimationFactory;

	class AnimationManagerImpl;

	class AnimationManager
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

		AnimationBase *GetNewAnimation(const std::string &animationType, AnimationProperties *properties = NULL);
		template<class T>
		T *GetNewAnimation(const std::string &animationType, AnimationProperties *properties = NULL)
		{
			return static_cast<T*>(GetNewAnimation(animationType, properties));
		}

		bool HasAnimation(const std::string &animationName);
		
		bool AddAnimation(const std::string &animationName, AnimationBase *animation);
		bool AddAnimationFromXML(const std::string &animationName, const std::string &filename);
		bool RemoveAnimation(const std::string &animationName);

		AnimationBase *GetAnimationFromName(const std::string &animationName);

		AnimationBase *RunAnimation(const std::string &animationName, AnimablePtr object);
		void RunAnimationDirect(AnimationBase *animation, AnimablePtr object);

		void Step(float deltaTime);
	private:
		AnimationManagerImpl *p;

		AnimationFactory &getAnimationFactory();
	};
}

#endif // AnimationManager_h__