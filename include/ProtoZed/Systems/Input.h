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

#include <ProtoZed/System.h>
#include <ProtoZed/Vector2.h>

namespace PZ
{
	namespace Key
	{
		enum Code
		{
			A = 'a',
			B = 'b',
			C = 'c',
			D = 'd',
			E = 'e',
			F = 'f',
			G = 'g',
			H = 'h',
			I = 'i',
			J = 'j',
			K = 'k',
			L = 'l',
			M = 'm',
			N = 'n',
			O = 'o',
			P = 'p',
			Q = 'q',
			R = 'r',
			S = 's',
			T = 't',
			U = 'u',
			V = 'v',
			W = 'w',
			X = 'x',
			Y = 'y',
			Z = 'z',
			Num0 = '0',
			Num1 = '1',
			Num2 = '2',
			Num3 = '3',
			Num4 = '4',
			Num5 = '5',
			Num6 = '6',
			Num7 = '7',
			Num8 = '8',
			Num9 = '9', 
			Escape = 256,
			LControl,
			LShift,
			LAlt,
			LSystem,      ///< OS specific key (left side) : windows (Win and Linux), apple (MacOS), ...
			RControl,
			RShift,
			RAlt,
			RSystem,      ///< OS specific key (right side) : windows (Win and Linux), apple (MacOS), ...
			Menu,
			LBracket,     ///< [
			RBracket,     ///< ]
			SemiColon,    ///< ;
			Comma,        ///< ,
			Period,       ///< .
			Quote,        ///< '
			Slash,        ///< /
			BackSlash,
			Tilde,        ///< ~
			Equal,        ///< =
			Dash,         ///< -
			Space,
			Return,
			Back,
			Tab,
			PageUp,
			PageDown,
			End,
			Home,
			Insert,
			Delete,
			Add,          ///< +
			Subtract,     ///< -
			Multiply,     ///< *
			Divide,       ///< /
			Left,         ///< Left arrow
			Right,        ///< Right arrow
			Up,           ///< Up arrow
			Down,         ///< Down arrow
			Numpad0,
			Numpad1,
			Numpad2,
			Numpad3,
			Numpad4,
			Numpad5,
			Numpad6,
			Numpad7,
			Numpad8,
			Numpad9,
			F1,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,
			F13,
			F14,
			F15,
			Pause,

			Count // Keep last -- total number of keyboard keys
		};
	}

	namespace Mouse
	{
		enum Button
		{
			Left,
			Right,
			Middle,
			XButton1,
			XButton2,

			ButtonCount // Keep last -- total number of mouse buttons
		};
	}

	namespace Joy
	{
		enum Axis
		{
			AxisX,
			AxisY,
			AxisZ,
			AxisR,
			AxisU,
			AxisV,
			AxisPOV,

			AxisCount // Keep last -- total number of joystick axis
		};

		enum
		{
			Count       = 4, ///< Total number of supported joysticks
			ButtonCount = 32 ///< Total number of supported joystick buttons
		};
	}

	namespace Systems
	{
		static const SystemType INPUT = "Input";
	}

	class Input : public System
	{
	public:
		Input(const SystemType &type, Application &application) : System(type, application)
		{}
		virtual ~Input()
		{}
	
		virtual void Update(float deltaTime) = 0;

		virtual bool IsKeyDown(Key::Code keyCode) const = 0;
	
		virtual float GetMouseX() const = 0;
		virtual float GetMouseY() const = 0;
		virtual bool IsMouseButtonDown(Mouse::Button button) const = 0;
	
		virtual float GetJoystickAxis(unsigned int joyId, Joy::Axis axis) const = 0;
		virtual bool IsJoystickButtonDown(unsigned int joyId, unsigned int button) const = 0;
	};
}

#endif // PZ_Input_h__
