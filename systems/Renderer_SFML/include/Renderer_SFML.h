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
#ifndef PZ_Renderer_SFML_h__
#define PZ_Renderer_SFML_h__

#include <ProtoZed/Systems/Renderer.h>
#include <ProtoZed/EntityManager.h>

namespace sf
{
	class RenderWindow;
}

namespace PZ
{
	class Renderer_SFML : public Renderer
	{
	public:
		Renderer_SFML(const SystemType &type, Application &application);
		~Renderer_SFML();

		virtual bool Start();
		virtual bool Stop();

		virtual void Update(float deltaTime);

		//FIXME: Highly temporary
		sf::RenderWindow &GetWindow() const;

	protected:
		virtual void videoModeUpdated();

	private:
		void OnEntityEvent(const EntityEvent &e);
		void OnComponentEvent(const ComponentEvent &e);
		void OnEntitiesCleared(const EntitiesClearedEvent &e);

		void EntityDestroyed(const EntityID &id);
		void ComponentAdded(const EntityID &id, const HashString &family);
		void ComponentRemoved(const EntityID &id, const HashString &family);

		class Impl;
		Impl *p;
	};
}

#endif // PZ_Renderer_SFML_h__
