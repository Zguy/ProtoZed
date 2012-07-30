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
#include <Input_SFML.h>

#include <ProtoZed/Application.h>
#include <ProtoZed/SystemList.h>

#include <Renderer_SFML.h>

#include <SFML/Graphics/RenderWindow.hpp>

namespace PZ
{
	Input_SFML::Input_SFML(const SystemType &type, Application &application) : Input(type, application), myMousePos(0.f, 0.f)
	{
		ResetStates();
	}
	Input_SFML::~Input_SFML()
	{
	}

	void Input_SFML::Update(float deltaTime)
	{
		//FIXME: Highly temporary
		Renderer_SFML *renderer = GetApplication().GetSystemList().Get<Renderer_SFML>(Systems::RENDERER);
		if (renderer != nullptr)
		{
			const sf::Input &input = renderer->GetWindow().GetInput();

			for (int i = 0; i < sf::Key::Count; ++i)
				myKeys[i] = input.IsKeyDown(static_cast<sf::Key::Code>(i));

			for (int i = 0; i < sf::Mouse::ButtonCount; ++i)
			{
				bool newDown = input.IsMouseButtonDown(static_cast<sf::Mouse::Button>(i));
				if (newDown != myMouseButtons[i])
				{
					EmitEvent(MouseButtonEvent(newDown, static_cast<Mouse::Button>(i)));
				}

				myMouseButtons[i] = newDown;
			}

			for (unsigned int i = 0; i < sf::Joy::Count; ++i)
			{
				for (unsigned int j = 0; j < sf::Joy::ButtonCount; ++j)
					myJoystickButtons[i][j] = input.IsJoystickButtonDown(i, j);

				for (int j = 0; j < sf::Joy::AxisCount; ++j)
					myJoystickAxis[i][j] = input.GetJoystickAxis(i, static_cast<sf::Joy::Axis>(j));
				myJoystickAxis[i][sf::Joy::AxisPOV] = input.GetJoystickAxis(i, sf::Joy::AxisPOV);
			}

			const sf::Vector2f mouse = renderer->GetWindow().ConvertCoords(input.GetMouseX(), input.GetMouseY());
			Vector2f newMouse(mouse.x, mouse.y);
			if (myMousePos != newMouse)
			{
				EmitEvent(MouseMoveEvent(myMousePos - newMouse));
			}

			myMousePos = newMouse;
		}
	}

	bool Input_SFML::IsKeyDown(Key::Code keyCode) const
	{
		return myKeys[keyCode];
	}

	float Input_SFML::GetMouseX() const
	{
		return myMousePos.x;
	}
	float Input_SFML::GetMouseY() const
	{
		return myMousePos.y;
	}
	bool Input_SFML::IsMouseButtonDown(Mouse::Button button) const
	{
		return myMouseButtons[button];
	}

	float Input_SFML::GetJoystickAxis(unsigned int joyId, Joy::Axis axis) const
	{
		if (joyId < Joy::Count)
			return myJoystickAxis[joyId][axis];
		else
			return 0.f;
	}
	bool Input_SFML::IsJoystickButtonDown(unsigned int joyId, unsigned int button) const
	{
		if ((joyId < Joy::Count) && (button < Joy::ButtonCount))
			return myJoystickButtons[joyId][button];
		else
			return false;
	}

	void Input_SFML::ResetStates()
	{
		for (int i = 0; i < Key::Count; ++i)
			myKeys[i] = false;

		for (int i = 0; i < Mouse::ButtonCount; ++i)
			myMouseButtons[i] = false;

		for (int i = 0; i < Joy::Count; ++i)
		{
			for (int j = 0; j < Joy::ButtonCount; ++j)
				myJoystickButtons[i][j] = false;

			for (int j = 0; j < Joy::AxisCount; ++j)
				myJoystickAxis[i][j] = 0.f;
			myJoystickAxis[i][Joy::AxisPOV] = -1.f;
		}
	}
}