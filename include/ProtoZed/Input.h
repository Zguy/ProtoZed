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
#ifndef Input_h__
#define Input_h__

#include <SFML/Graphics/RenderWindow.hpp>

namespace PZ
{
	class Input
	{
		friend class Application;

	public:
		Input();
	
		bool IsKeyDown(sf::Key::Code KeyCode) const;
	
		float GetMouseX() const;
		float GetMouseY() const;
		bool IsMouseButtonDown(sf::Mouse::Button Button) const;
	
		float GetJoystickAxis(unsigned int JoyId, sf::Joy::Axis Axis) const;
		bool IsJoystickButtonDown(unsigned int JoyId, unsigned int Button) const;
	
		Input &operator=(const Input &rhs);
	
		bool operator==(const Input &rhs);
		bool operator!=(const Input &rhs);
	
	private:
		void Update(const sf::RenderWindow &window);

		void ResetStates();
	
		bool  myKeys[sf::Key::Count];
		bool  myMouseButtons[sf::Mouse::ButtonCount];
		float myMouseX;
		float myMouseY;
		bool  myJoystickButtons[sf::Joy::Count][sf::Joy::ButtonCount];
		float myJoystickAxis[sf::Joy::Count][sf::Joy::AxisCount];
	};
}

#endif // Input_h__