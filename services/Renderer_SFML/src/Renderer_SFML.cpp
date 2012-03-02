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
#include <Renderer_SFML.h>

#include <sfImageAsset.h>

#include <ProtoZed/Components/Sprite.h>
#include <ProtoZed/Components/Position2D.h>

#include <ProtoZed/Application.h>
#include <ProtoZed/AssetManager.h>

#include <SFML/Graphics.hpp>

#include <queue>

namespace PZ
{
	struct Layer
	{
		Layer(const EntityID &id, sf::Drawable *drawable, float depth) : id(id), drawable(drawable), depth(depth), used(true)
		{}
		~Layer()
		{}

		EntityID id;
		sf::Drawable *drawable;
		float depth;

		bool used;

		bool operator<(const Layer &other) const
		{
			return depth < other.depth;
		}
	};

	typedef std::vector<Layer> LayerList;

	class Renderer_SFML::Impl
	{
	public:
		Impl() : isFullscreen(false), clearLayerAcc(0.f), clearLayerLimit(60.f)
		{}

		void calculateView(const VideoMode &videoMode)
		{
			view.SetFromRect(sf::FloatRect(0.f,0.f,static_cast<float>(videoMode.GetVirtualResolution().x),static_cast<float>(videoMode.GetVirtualResolution().y)));
		}

		LayerList::iterator findLayer(const EntityID &id)
		{
			for (LayerList::iterator it = layers.begin(); it != layers.end(); ++it)
			{
				if ((*it).id == id)
				{
					return it;
				}
			}
			return layers.end();
		}

		LayerList::iterator newLayer(const EntityID &id, sf::Drawable *drawable, float depth)
		{
			/*for (LayerList::iterator it = layers.begin(); it != layers.end(); ++it)
			{
				if (!(*it).used)
				{
					Layer &layer = (*it);
					layer.id = id;
					delete layer.drawable;
					layer.drawable = drawable;
					layer.depth = depth;
					layer.used = true;

					return it;
				}
			}*/

			// No unused layer found
			Layer layer(id, drawable, depth);
			layers.push_back(layer);

			return layers.end()-1;
		}

		sf::View view;
		bool isFullscreen;
		sf::RenderWindow window;

		LayerList layers;

		float clearLayerAcc;
		float clearLayerLimit;
	};

	Renderer_SFML::Renderer_SFML(const ServiceType &type, Application &application) : Renderer(type, application), p(new Impl)
	{
	}
	Renderer_SFML::~Renderer_SFML()
	{
		for (LayerList::iterator it = p->layers.begin(); it != p->layers.end(); ++it)
		{
			delete (*it).drawable;
			(*it).drawable = nullptr;
		}
		p->layers.clear();

		delete p;
	}

	bool Renderer_SFML::Start()
	{
		if (!Renderer::Start())
			return false;

		unsigned long windowStyle = (sf::Style::Close | sf::Style::Resize);

		sf::VideoMode sfVideoMode(videoMode.GetWindowResolution().x, videoMode.GetWindowResolution().y);

		if (videoMode.GetFullscreen())
		{
			windowStyle |= sf::Style::Fullscreen;
			p->isFullscreen = true;
		}

		p->window.Create(sfVideoMode, title, windowStyle);

		p->calculateView(videoMode);
		p->window.SetView(p->view);

		return true;
	}
	bool Renderer_SFML::Stop()
	{
		if (!Renderer::Stop())
			return false;

		p->window.Close();

		return true;
	}

	void Renderer_SFML::Update(float deltaTime)
	{
		Profiler &profiler = Profiler::GetSingleton();

		profiler.Begin("PollEvent");
		sf::Event event;
		while (p->window.GetEvent(event))
		{
			if (event.Type == sf::Event::Closed)
			{
				GetApplication().RequestShutdown();
			}
		}
		profiler.End();

		profiler.Begin("ClearWindow");
		p->window.Clear();
		profiler.End();

		p->clearLayerAcc += deltaTime;
		if (p->clearLayerAcc >= p->clearLayerLimit)
		{
			p->clearLayerAcc -= p->clearLayerLimit;

			profiler.Begin("ClearUnusedLayers");
			for (LayerList::iterator it = p->layers.begin(); it != p->layers.end();)
			{
				if (!(*it).used)
				{
					delete (*it).drawable;
					it = p->layers.erase(it);
				}
				else
				{
					(*it).used = false;
					++it;
				}
			}
			profiler.End();
		}
		else
		{
			profiler.Begin("MarkLayersUnused");
			for (LayerList::iterator it = p->layers.begin(); it != p->layers.end(); ++it)
			{
				(*it).used = false;
			}
			profiler.End();
		}

		profiler.Begin("IterateEntities");
		{
			Profile profile("Sprite");

			const EntityComponentMap &ecm = GetApplication().GetEntityManager().GetEntitiesWith<Sprite>();
			for (EntityComponentMap::const_iterator it = ecm.cbegin(); it != ecm.cend(); ++it)
			{
				EntityID id = (*it).first;
				Sprite *sprite = static_cast<Sprite*>((*it).second);

				profiler.Begin("FindSprite");
				LayerList::iterator layerIt = p->findLayer(id);
				//TODO: What if the sprite has changed?
				if (layerIt == p->layers.end())
				{
					profiler.Begin("CreateLayer");

					sf::Sprite *sfSprite = nullptr;

					const sfImageAsset *image = GetApplication().GetAssetManager().Get<sfImageAsset>(sprite->GetSprite());
					if (image != nullptr)
					{
						sfSprite = new sf::Sprite(image->GetImage());
					}
					else
					{
						sfSprite = new sf::Sprite;
					}

					const Vector2f &center = sprite->GetCenter();
					sfSprite->SetCenter(sf::Vector2f(center.x, center.y));

					layerIt = p->newLayer(id, sfSprite, 0);
					profiler.End();
				}
				Layer &layer = (*layerIt);
				layer.used = true;
				profiler.End();

				profiler.Begin("GetPosition");
				Position2D *position = GetApplication().GetEntityManager().GetComponent<Position2D>(id);
				if (position != nullptr)
				{
					const Vector2f &pos = position->GetPosition(Scope::GLOBAL);
					layer.drawable->SetPosition(sf::Vector2f(pos.x, pos.y));
					layer.drawable->SetRotation(position->GetRotation(Scope::GLOBAL).ToFloat());

					layer.depth = position->GetDepth();
				}
				profiler.End();
			}
		}
		profiler.End();

		profiler.Begin("SortLayers");
		std::sort(p->layers.begin(), p->layers.end());
		profiler.End();

		profiler.Begin("DrawLayers");
		for (LayerList::const_iterator it = p->layers.cbegin(); it != p->layers.cend(); ++it)
		{
			const Layer &layer = (*it);

			if (layer.used)
			{
				p->window.Draw(*layer.drawable);
			}
		}
		profiler.End();

		profiler.Begin("Display");
		p->window.Display();
		profiler.End();
	}

	void Renderer_SFML::videoModeUpdated()
	{
		if (IsStarted())
		{
			p->window.SetSize(videoMode.GetWindowResolution().x, videoMode.GetWindowResolution().y);
			p->calculateView(videoMode);

			if (p->isFullscreen != videoMode.GetFullscreen())
			{
				// Restart renderer
				Stop();
				Start();
			}
		}
	}

	sf::RenderWindow &Renderer_SFML::GetWindow() const
	{
		return p->window;
	}
}