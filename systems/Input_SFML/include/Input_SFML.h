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
#ifndef PZ_Input_SFML_h__
#define PZ_Input_SFML_h__

#include <ProtoZed/Systems/Input.h>

namespace PZ
{
	class Input_SFML : public Input
	{
	public:
		Input_SFML(const SystemType &type, Application &application);
		~Input_SFML();

		virtual void Update(float deltaTime);

		virtual bool IsKeyDown(Key::Code keyCode) const;

		virtual float GetMouseX() const;
		virtual float GetMouseY() const;
		virtual bool IsMouseButtonDown(Mouse::Button button) const;

		virtual float GetJoystickAxis(unsigned int joyId, Joy::Axis axis) const;
		virtual bool IsJoystickButtonDown(unsigned int joyId, unsigned int button) const;

	private:
		void ResetStates();

		bool  myKeys[Key::Count];
		bool  myMouseButtons[Mouse::ButtonCount];
		float myMouseX;
		float myMouseY;
		bool  myJoystickButtons[Joy::Count][Joy::ButtonCount];
		float myJoystickAxis[Joy::Count][Joy::AxisCount];
	};
}

#endif // PZ_Input_SFML_h__