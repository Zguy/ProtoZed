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
#include <ProtoZed/Input.h>

namespace PZ
{
	Input::Input() : myMouseX(0), myMouseY(0)
	{
		ResetStates();
	}

	bool Input::IsKeyDown(sf::Key::Code KeyCode) const
	{
		return myKeys[KeyCode];
	}
	
	float Input::GetMouseX() const
	{
		return myMouseX;
	}
	float Input::GetMouseY() const
	{
		return myMouseY;
	}
	bool Input::IsMouseButtonDown(sf::Mouse::Button Button) const
	{
		return myMouseButtons[Button];
	}
	
	float Input::GetJoystickAxis(unsigned int JoyId, sf::Joy::Axis Axis) const
	{
		if (JoyId < sf::Joy::Count)
			return myJoystickAxis[JoyId][Axis];
		else
			return 0.f;
	}
	bool Input::IsJoystickButtonDown(unsigned int JoyId, unsigned int Button) const
	{
		if ((JoyId < sf::Joy::Count) && (Button < sf::Joy::ButtonCount))
			return myJoystickButtons[JoyId][Button];
		else
			return false;
	}
	
	Input &Input::operator=(const Input &rhs)
	{
		if (this == &rhs)
			return *this;
	
		for (int i = 0; i < sf::Key::Count; ++i)
			myKeys[i] = rhs.myKeys[i];
	
		for (int i = 0; i < sf::Mouse::ButtonCount; ++i)
			myMouseButtons[i] = rhs.myMouseButtons[i];
	
		for (int i = 0; i < sf::Joy::Count; ++i)
		{
			for (int j = 0; j < sf::Joy::ButtonCount; ++j)
				myJoystickButtons[i][j] = rhs.myJoystickButtons[i][j];
	
			for (int j = 0; j < sf::Joy::AxisCount; ++j)
				myJoystickAxis[i][j] = rhs.myJoystickAxis[i][j];
			myJoystickAxis[i][sf::Joy::AxisPOV] = rhs.myJoystickAxis[i][sf::Joy::AxisPOV];
		}
	
		myMouseX = rhs.myMouseX;
		myMouseY = rhs.myMouseY;
	
		return *this;
	}
	
	bool Input::operator==(const Input &rhs)
	{
		bool equal = true;
	
		for (int i = 0; i < sf::Key::Count; ++i)
			if (myKeys[i] != rhs.myKeys[i])
				equal = false;
	
		for (int i = 0; i < sf::Mouse::ButtonCount; ++i)
			if (myMouseButtons[i] != rhs.myMouseButtons[i])
				equal = false;
	
		for (int i = 0; i < sf::Joy::Count; ++i)
		{
			for (int j = 0; j < sf::Joy::ButtonCount; ++j)
				if (myJoystickButtons[i][j] != rhs.myJoystickButtons[i][j])
					equal = false;
	
			for (int j = 0; j < sf::Joy::AxisCount; ++j)
				if (myJoystickAxis[i][j] != rhs.myJoystickAxis[i][j])
					equal = false;
			if (myJoystickAxis[i][sf::Joy::AxisPOV] != rhs.myJoystickAxis[i][sf::Joy::AxisPOV])
				equal = false;
		}
	
		if (myMouseX != rhs.myMouseX)
			equal = false;
		if (myMouseY != rhs.myMouseY)
			equal = false;
	
		return equal;
	}
	bool Input::operator!=(const Input &rhs)
	{
		return !(*this == rhs);
	}

	void Input::Update(const sf::RenderWindow &window)
	{
		const sf::Input &input = window.GetInput();

		for (int i = 0; i < sf::Key::Count; ++i)
			myKeys[i] = input.IsKeyDown(static_cast<sf::Key::Code>(i));

		for (int i = 0; i < sf::Mouse::ButtonCount; ++i)
			myMouseButtons[i] = input.IsMouseButtonDown(static_cast<sf::Mouse::Button>(i));

		for (int i = 0; i < sf::Joy::Count; ++i)
		{
			for (int j = 0; j < sf::Joy::ButtonCount; ++j)
				myJoystickButtons[i][j] = input.IsJoystickButtonDown(i, j);

			for (int j = 0; j < sf::Joy::AxisCount; ++j)
				myJoystickAxis[i][j] = input.GetJoystickAxis(i, static_cast<sf::Joy::Axis>(j));
			myJoystickAxis[i][sf::Joy::AxisPOV] = input.GetJoystickAxis(i, sf::Joy::AxisPOV);
		}

		const sf::Vector2f mouse = window.ConvertCoords(input.GetMouseX(), input.GetMouseY());
		myMouseX = mouse.x;
		myMouseY = mouse.y;
	}

	void Input::ResetStates()
	{
		for (int i = 0; i < sf::Key::Count; ++i)
			myKeys[i] = false;
	
		for (int i = 0; i < sf::Mouse::ButtonCount; ++i)
			myMouseButtons[i] = false;
	
		for (int i = 0; i < sf::Joy::Count; ++i)
		{
			for (int j = 0; j < sf::Joy::ButtonCount; ++j)
				myJoystickButtons[i][j] = false;
	
			for (int j = 0; j < sf::Joy::AxisCount; ++j)
				myJoystickAxis[i][j] = 0.f;
			myJoystickAxis[i][sf::Joy::AxisPOV] = -1.f;
		}
	}
}