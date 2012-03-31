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
#include <ProtoZed/Components/SoundEmitter.h>

namespace PZ
{
	const HashString SoundEmitter::Family = "SoundEmitter";

	SoundEmitter::SoundEmitter(const EntityID &owner, EntityManager &manager) : Component(owner, manager), sound("Sound"), volume("Volume", 100.f), playing("Playing", false), repeat("Repeat", false), minDistance("MinDistance", 1.f), attenuation("Attenuation", 1.f)
	{
		AddProperty(sound);
		AddProperty(volume);
		AddProperty(playing);
		AddProperty(repeat);

		AddProperty(minDistance);
		AddProperty(attenuation);
	}
	SoundEmitter::~SoundEmitter()
	{
	}

	void SoundEmitter::Play()
	{
		playing = true;
	}
	void SoundEmitter::Stop()
	{
		playing = false;
	}
}