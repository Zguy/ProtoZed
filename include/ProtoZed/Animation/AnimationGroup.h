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
#ifndef AnimationGroup_h__
#define AnimationGroup_h__

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

	class AnimationGroupImpl;

	class AnimationGroup : public AnimationBase
	{
	public:
		AnimationGroup();
		AnimationGroup(AnimationProperties *properties);
		AnimationGroup(const AnimationGroup &copy);
		virtual ~AnimationGroup();

		void SetAsync(bool async);

		void AddAnimation(AnimationBase *animation);
		void AddAnimation(const std::string &animationType, AnimationProperties *properties);

		virtual AnimationBase *GetCopy();

		virtual void AddTime(float deltaTime);

	protected:
		virtual bool StartImpl();

	private:
		AnimationGroupImpl *p;
	};
}

#endif // AnimationGroup_h__