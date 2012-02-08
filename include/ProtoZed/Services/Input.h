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
#ifndef PZ_Input_h__
#define PZ_Input_h__

#include <ProtoZed/Service.h>

#include <SFML/Graphics/RenderWindow.hpp>

namespace PZ
{
	class Input : public Service
	{
	public:
		Input(const ServiceType &type, Application &application);
		~Input();
	
		virtual void Update(float deltaTime) = 0;

		bool IsKeyDown(sf::Key::Code KeyCode) const;
	
		float GetMouseX() const;
		float GetMouseY() const;
		bool IsMouseButtonDown(sf::Mouse::Button Button) const;
	
		float GetJoystickAxis(unsigned int JoyId, sf::Joy::Axis Axis) const;
		bool IsJoystickButtonDown(unsigned int JoyId, unsigned int Button) const;
	
		Input &operator=(const Input &rhs);
	
		bool operator==(const Input &rhs);
		bool operator!=(const Input &rhs);
	
	protected:
		void ResetStates();
	
		bool  myKeys[sf::Key::Count];
		bool  myMouseButtons[sf::Mouse::ButtonCount];
		float myMouseX;
		float myMouseY;
		bool  myJoystickButtons[sf::Joy::Count][sf::Joy::ButtonCount];
		float myJoystickAxis[sf::Joy::Count][sf::Joy::AxisCount];
	};
}

#endif // PZ_Input_h__