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
#ifndef PZ_Spritesheet_h__
#define PZ_Spritesheet_h__

#include <ProtoZed/Component.h>
#include <ProtoZed/Assets/SpritesheetAsset.h>

namespace PZ
{
	class Spritesheet : public Component
	{
	public:
		static const HashString Family;

		Spritesheet();
		~Spritesheet();

		bool SetSpritesheet(const std::string &spritesheet);

		bool SetAnimation(const std::string &animation);
		void SetFrame(unsigned int frame);

		void Play();
		void Stop();

		virtual void Update(float deltaTime);

	protected:
		virtual void PropertyUpdated(const PropertyBase &prop);

	private:
		Property<std::string> spritesheet;

		Property<std::string> animation;
		Property<unsigned int> frame;

		Property<bool> playing;

		const SpritesheetAsset *sheet;
		const SpritesheetAsset::Animation *anim;
		float currentTime;
	};
}

#endif // PZ_Spritesheet_h__