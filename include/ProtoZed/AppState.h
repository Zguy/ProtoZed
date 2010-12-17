#ifndef AppState_h__
#define AppState_h__

#include <map>
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>

namespace PZ
{
	typedef std::map<std::string, std::string> StringMap;

	class AppState
	{
	public:
		AppState() : started(false)
		{}
		virtual ~AppState()
		{}

		virtual bool Update(float deltaTime) = 0;
		virtual void Draw(sf::RenderWindow &window) = 0;

		virtual void Start(StringMap *const options) { started = true; }
		virtual void Stop() { started = false; }

		virtual void Pause() {};
		virtual void Resume() {};

		virtual void OnKeyDown(const sf::Event::KeyEvent &keyEvent) {}
		virtual void OnKeyUp(const sf::Event::KeyEvent &keyEvent) {}
		virtual void OnMouseDown(const sf::Event::MouseButtonEvent &mouseButtonEvent) {}
		virtual void OnMouseUp(const sf::Event::MouseButtonEvent &mouseButtonEvent) {}
		virtual void OnMouseMove(const sf::Event::MouseMoveEvent &mouseMoveEvent) {}

		virtual void LoadAssets() {};
		virtual void UnloadAssets() {};

		inline bool IsStarted() { return started; }

	protected:
		bool started;
	};
}

#endif // AppState_h__