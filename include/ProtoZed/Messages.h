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
#ifndef PZ_Messages_h__
#define PZ_Messages_h__

#include <ProtoZed/Message.h>

namespace PZ
{
	namespace Messages
	{
	}

	/*struct UpdateMessage
	{
		UpdateMessage(const Message &message)
		{
			deltaTime = static_cast<float>(message.args.Get("deltaTime", Jzon::Value(0.f)).AsDouble());
		}

		static Message Create(float deltaTime)
		{
			Message message(Messages::UPDATE);
			message.args.Add("deltaTime", deltaTime);

			return message;
		}

		float deltaTime;
	};*/
}

#endif // PZ_Messages_h__