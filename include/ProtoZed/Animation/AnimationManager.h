/*
	Copyright 2010 Johannes H�ggqvist

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
#ifndef AnimationManager_h__
#define AnimationManager_h__

#include <string>

#include <ProtoZed/Animation/Animable.h>

#include <ProtoZed/ObjectFactory/ObjectFactory.h>

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

		AnimationBase *GetNewAnimation(const std::string &animationType, AnimationProperties *properties = NULL) const;
		template<class T>
		T *GetNewAnimation(const std::string &animationType, AnimationProperties *properties = NULL) const
		{
			return static_cast<T*>(GetNewAnimation(animationType, properties));
		}

		bool HasAnimation(const std::string &animationName) const;
		
		bool AddAnimation(const std::string &animationName, AnimationBase *animation);
		bool AddAnimationFromXML(const std::string &animationName, const std::string &filename);
		bool RemoveAnimation(const std::string &animationName);

		AnimationBase *GetAnimationFromName(const std::string &animationName) const;

		AnimationBase *RunAnimation(const std::string &animationName, AnimablePtr object);
		void RunAnimationDirect(AnimationBase *animation, AnimablePtr object);

		void Step(float deltaTime);
	private:
		AnimationManagerImpl *p;

		AnimationFactory &getAnimationFactory() const;
	};
}

#endif // AnimationManager_h__