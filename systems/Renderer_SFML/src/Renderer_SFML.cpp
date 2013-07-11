/*
Copyright (c) 2013 Johannes Häggqvist

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
#include <ProtoZed/AssetLibrary.h>

#include <SFML/Graphics.hpp>

#include <queue>

namespace PZ
{
	struct Layer
	{
		Layer(const EntityID &id, sf::Drawable *drawable, float depth) : id(id), drawable(drawable), depth(depth)
		{}
		~Layer()
		{}

		EntityID id;
		sf::Drawable *drawable;
		float depth;

		Timestamp timestamp;

		bool operator<(const Layer &other) const
		{
			return depth < other.depth;
		}

		bool operator==(const EntityID &id) const
		{
			return this->id == id;
		}
	};

	typedef std::vector<Layer> LayerList;

	class Renderer_SFML::Impl
	{
	public:
		Impl(Renderer_SFML *i) : i(i), isFullscreen(false)
		{}

		void calculateView(const VideoMode &videoMode)
		{
			view.SetFromRect(sf::FloatRect(0.f,0.f,static_cast<float>(videoMode.GetVirtualResolution().x),static_cast<float>(videoMode.GetVirtualResolution().y)));
		}

		LayerList::iterator findLayer(const EntityID &id)
		{
			return std::find(layers.begin(), layers.end(), id);
		}

		void updateLayer(Layer &layer, const Sprite *sprite)
		{
			AssetLibrary &assetLibrary = i->GetApplication().GetAssetLibrary();

			sf::Sprite *drawable = dynamic_cast<sf::Sprite*>(layer.drawable);
			if (drawable == nullptr)
			{
				delete layer.drawable;
				drawable = new sf::Sprite;
				layer.drawable = drawable;
			}

			if (!sprite->GetImage().empty())
			{
				const sfImageAsset *image = assetLibrary.Get<sfImageAsset>(sprite->GetImage());
				if (image != nullptr)
				{
					drawable->SetImage(image->GetImage());
				}
			}

			const Color &color = sprite->GetColor();
			drawable->SetColor(sf::Color(color.GetRed() * 255.f, color.GetGreen() * 255.f, color.GetBlue() * 255.f, color.GetAlpha() * 255.f));

			drawable->SetCenter(sprite->GetCenter().x, sprite->GetCenter().y);
			if (sprite->GetRegionSize().GetLengthSquared() > 0.f)
			{
				int x = static_cast<int>(sprite->GetRegionOffset().x);
				int y = static_cast<int>(sprite->GetRegionOffset().y);
				int w = static_cast<int>(sprite->GetRegionSize().x);
				int h = static_cast<int>(sprite->GetRegionSize().y);
				drawable->SetSubRect(sf::IntRect(x, y, x+w, y+h));
			}
		}
		LayerList::iterator createLayer(const EntityID &id, const Sprite *sprite)
		{
			Layer layer(id, nullptr, 0);

			updateLayer(layer, sprite);

			layers.push_back(layer);

			return layers.end()-1;
		}
		void deleteLayer(const EntityID &id)
		{
			LayerList::iterator it = findLayer(id);
			if (it != layers.end())
			{
				delete (*it).drawable;
				
				layers.erase(it);
			}
		}
		void clearLayers()
		{
			for (LayerList::iterator it = layers.begin(); it != layers.end(); ++it)
			{
				delete (*it).drawable;
				(*it).drawable = nullptr;
			}
			layers.clear();
		}

		Renderer_SFML *i;

		sf::View view;
		bool isFullscreen;
		sf::RenderWindow window;

		LayerList layers;
	};

	Renderer_SFML::Renderer_SFML(const SystemType &type, Application &application) : Renderer(type, application)
	{
		p = new Impl(this);
	}
	Renderer_SFML::~Renderer_SFML()
	{
		Stop();

		delete p;
	}

	bool Renderer_SFML::OnStart()
	{
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

		EntityManager &entityManager = GetApplication().GetEntityManager();
		entityManager.Subscribe(EntityEvent::DESTROYED, this, &Renderer_SFML::OnEntityDestroyed);
		entityManager.Subscribe(EntityEvent::CLEARED, this, &Renderer_SFML::OnEntitiesCleared);
		entityManager.Subscribe(ComponentEvent::ADDED, this, &Renderer_SFML::OnComponentAdded);
		entityManager.Subscribe(ComponentEvent::REMOVED, this, &Renderer_SFML::OnComponentRemoved);

		return true;
	}
	bool Renderer_SFML::OnStop()
	{
		EntityManager &entityManager = GetApplication().GetEntityManager();
		entityManager.Unsubscribe(EntityEvent::DESTROYED, this, &Renderer_SFML::OnEntityDestroyed);
		entityManager.Unsubscribe(EntityEvent::CLEARED, this, &Renderer_SFML::OnEntitiesCleared);
		entityManager.Unsubscribe(ComponentEvent::ADDED, this, &Renderer_SFML::OnComponentAdded);
		entityManager.Unsubscribe(ComponentEvent::REMOVED, this, &Renderer_SFML::OnComponentRemoved);

		p->clearLayers();

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

		profiler.Begin("IterateEntities");
		{
			Profile profile("Sprite");

			const EntityComponentMap &ecm = GetApplication().GetEntityManager().GetEntitiesWith<Sprite>();
			for (EntityComponentMap::const_iterator it = ecm.cbegin(); it != ecm.cend(); ++it)
			{
				const EntityID &id = (*it).first;
				const Sprite *sprite = static_cast<Sprite*>((*it).second);

				profiler.Begin("FindSprite");
				LayerList::iterator layerIt = p->findLayer(id);
				if (layerIt == p->layers.end())
				{
					profiler.Begin("CreateLayer");

					layerIt = p->createLayer(id, sprite);

					profiler.End();
				}

				Layer &layer = (*layerIt);

				profiler.End();

				profiler.Begin("CheckTimestamp");

				Timestamp &timestamp = layer.timestamp;
				if (sprite->GetTimestamp() != timestamp)
				{
					p->updateLayer(layer, sprite);
					timestamp = sprite->GetTimestamp();
				}

				profiler.End();

				profiler.Begin("GetPosition");
				Position2D *position = GetApplication().GetEntityManager().GetComponent<Position2D>(id);
				if (position != nullptr)
				{
					const Vector2f &pos   = position->GetPosition(Scope::GLOBAL);
					const Vector2f &scale = position->GetScale(Scope::GLOBAL);
					layer.drawable->SetPosition(sf::Vector2f(pos.x, pos.y));
					layer.drawable->SetRotation(position->GetRotation(Scope::GLOBAL).ToFloat());
					layer.drawable->SetScale(sf::Vector2f(scale.x, scale.y));

					layer.depth = position->GetDepth();
				}
				profiler.End();
			}
		}
		profiler.End();

		profiler.Begin("SortLayers");
		std::sort(p->layers.begin(), p->layers.end());
		profiler.End();

		profiler.Begin("ClearWindow");
		p->window.Clear();
		profiler.End();

		profiler.Begin("DrawLayers");
		for (LayerList::const_iterator it = p->layers.cbegin(); it != p->layers.cend(); ++it)
		{
			const Layer &layer = (*it);

			p->window.Draw(*layer.drawable);
		}
		profiler.End();

		profiler.Begin("Display");
		p->window.Display();
		profiler.End();
	}

	sf::RenderWindow &Renderer_SFML::GetWindow() const
	{
		return p->window;
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

	void Renderer_SFML::OnEntityDestroyed(const EntityEvent &e)
	{
		p->deleteLayer(e.Id);
	}
	void Renderer_SFML::OnEntitiesCleared(const EntityEvent &e)
	{
		p->clearLayers();
	}
	void Renderer_SFML::OnComponentAdded(const ComponentEvent &e)
	{
		if (e.Family == Sprite::Family)
		{
			const Sprite *sprite = GetApplication().GetEntityManager().GetComponent<Sprite>(e.Id);
			p->createLayer(e.Id, sprite);
		}
	}
	void Renderer_SFML::OnComponentRemoved(const ComponentEvent &e)
	{
		if (e.Family == Sprite::Family)
		{
			p->deleteLayer(e.Id);
		}
	}
}