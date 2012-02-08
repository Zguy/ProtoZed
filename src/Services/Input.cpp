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
#include <ProtoZed/Services/Input.h>

namespace PZ
{
	Input::Input(const ServiceType &type, Application &application) : Service(type, application), myMouseX(0), myMouseY(0)
	{
		ResetStates();
	}
	Input::~Input()
	{

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
		for (int i = 0; i < sf::Key::Count; ++i)
			if (myKeys[i] != rhs.myKeys[i])
				return false;
	
		for (int i = 0; i < sf::Mouse::ButtonCount; ++i)
			if (myMouseButtons[i] != rhs.myMouseButtons[i])
				return false;
	
		for (int i = 0; i < sf::Joy::Count; ++i)
		{
			for (int j = 0; j < sf::Joy::ButtonCount; ++j)
				if (myJoystickButtons[i][j] != rhs.myJoystickButtons[i][j])
					return false;
	
			for (int j = 0; j < sf::Joy::AxisCount; ++j)
				if (myJoystickAxis[i][j] != rhs.myJoystickAxis[i][j])
					return false;
			if (myJoystickAxis[i][sf::Joy::AxisPOV] != rhs.myJoystickAxis[i][sf::Joy::AxisPOV])
				return false;
		}
	
		if (myMouseX != rhs.myMouseX)
			return false;
		if (myMouseY != rhs.myMouseY)
			return false;

		return true;
	}
	bool Input::operator!=(const Input &rhs)
	{
		return !(*this == rhs);
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