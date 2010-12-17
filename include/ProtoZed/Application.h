#ifndef Application_h__
#define Application_h__

#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

#include "Singleton.h"
#include "AppStateManager.h"
#include "AssetManager.h"
#include "Animation/AnimationManager.h"

namespace PZ
{
	typedef AssetManager<sf::Image>       ImageManager;
	typedef AssetManager<sf::Font>        FontManager;
	typedef AssetManager<sf::SoundBuffer> SoundBufferManager;

	class ApplicationImpl;

	class Application : public Singleton<Application>
	{
	public:
		Application();
		~Application();

		int Run(const std::string &appName, sf::VideoMode &videoMode, sf::WindowSettings &params = sf::WindowSettings());

		void RequestShutdown();

		AppStateManager    &GetStateManager();
		sf::RenderWindow   &GetRenderWindow();
		const sf::Input    &GetInput();
		AnimationManager   &GetAnimationManager();
		ImageManager       &GetImageManager();
		FontManager        &GetFontManager();
		SoundBufferManager &GetSoundBufferManager();

		void LogMessage(const std::string &message);

	private:
		ApplicationImpl *p;
	};
}

#endif // Application_h__