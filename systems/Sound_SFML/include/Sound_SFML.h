/*
Copyright (c) 2012 Johannes H�ggqvist

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
#ifndef PZ_Sound_SFML_h__
#define PZ_Sound_SFML_h__

#include <ProtoZed/Systems/Sound.h>
#include <ProtoZed/EntityManager.h>

namespace PZ
{
	class Sound_SFML : public Sound, public EntityListener
	{
	public:
		Sound_SFML(const SystemType &type, Application &application);
		~Sound_SFML();

		virtual bool Start();
		virtual bool Stop();

		virtual void Update(float deltaTime);

	private:
		virtual void EntityDestroyedPost(const EntityID &id);

		virtual void EntitiesClearedPost();

		virtual void ComponentAddedPost(const EntityID &id, const HashString &family);
		virtual void ComponentRemovedPost(const EntityID &id, const HashString &family);

		class Impl;
		Impl *p;
	};
}

#endif // PZ_Sound_SFML_h__
