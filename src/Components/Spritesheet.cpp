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
#include <ProtoZed/Components/Spritesheet.h>

#include <ProtoZed/Application.h>
#include <ProtoZed/AssetManager.h>
#include <ProtoZed/Assets/ImageAsset.h>
#include <ProtoZed/Components/Sprite.h>

namespace PZ
{
	const HashString Spritesheet::Family = "Spritesheet";

	Spritesheet::Spritesheet() : spritesheet("Spritesheet"), animation("Animation"), frame("Frame", (unsigned int)0), playing("Playing", false), sheet(nullptr), anim(nullptr), currentTime(0.f)
	{
		AddProperty(spritesheet);
		AddProperty(animation);
		AddProperty(frame);
		AddProperty(playing);
	}
	Spritesheet::~Spritesheet()
	{
	}

	bool Spritesheet::SetSpritesheet(const std::string &spritesheet)
	{
		sheet = GetApplication().GetAssetManager().Get<SpritesheetAsset>(spritesheet);
		this->spritesheet.LockNotify();
		this->spritesheet = spritesheet;
		this->spritesheet.UnlockNotify();
		anim = nullptr;
		animation.LockNotify();
		animation = "";
		animation.UnlockNotify();
		frame.LockNotify();
		frame = 0;
		frame.UnlockNotify();
		currentTime = 0.f;

		if (sheet != nullptr)
		{
			Sprite *sprite = GetManager().GetComponent<PZ::Sprite>(GetOwnerID());
			if (sprite != nullptr)
			{
				sprite->SetImage(sheet->image);
			}
		}

		return (sheet != nullptr);
	}

	bool Spritesheet::SetAnimation(const std::string &animation)
	{
		if (sheet != nullptr)
		{
			SpritesheetAsset::AnimationMap::const_iterator it = sheet->animations.find(animation);
			if (it != sheet->animations.end())
			{
				anim = &(*it).second;
				this->animation.LockNotify();
				this->animation = animation;
				this->animation.UnlockNotify();
				currentTime = 0.f;
				SetFrame(anim->from);
				return true;
			}
			else
			{
				anim = nullptr;
				this->animation.LockNotify();
				this->animation = "";
				this->animation.UnlockNotify();
				currentTime = 0.f;
			}
		}

		return false;
	}
	void Spritesheet::SetFrame(unsigned int frame)
	{
		if (sheet != nullptr)
		{
			const ImageAsset *image = GetApplication().GetAssetManager().Get<ImageAsset>(sheet->image);
			if (image != nullptr)
			{
				const unsigned int imageWidth = image->GetWidth();
				const unsigned int imageHeight = image->GetHeight();
				if (imageWidth > 0 && imageHeight > 0)
				{
					const unsigned int frameWidth = sheet->size.x;
					const unsigned int frameHeight = sheet->size.y;

					const unsigned int framesPerRow = imageWidth / frameWidth;
					const unsigned int frameY = (frame/framesPerRow);

					const unsigned int x = (frame-(frameY*framesPerRow))*frameWidth;
					const unsigned int y = frameY*frameHeight;

					Sprite *sprite = GetManager().GetComponent<Sprite>(GetOwnerID());
					if (sprite != nullptr)
					{
						sprite->SetRegion(PZ::Vector2f(x, y), PZ::Vector2f(frameWidth, frameHeight));
					}

					this->frame.LockNotify();
					this->frame = frame;
					this->frame.UnlockNotify();
				}
			}
		}
	}

	void Spritesheet::Play()
	{
		playing = true;
	}
	void Spritesheet::Stop()
	{
		playing = false;
	}

	void Spritesheet::Update(float deltaTime)
	{
		if (playing)
		{
			currentTime += deltaTime;

			if (anim != nullptr && currentTime >= anim->time)
			{
				currentTime = 0.f;

				if (frame+1 > anim->to)
				{
					if (anim->loop)
						SetFrame(anim->from);
					else
						Stop();
				}
				else
				{
					SetFrame(frame+1);
				}
			}
		}
	}

	void Spritesheet::PropertyUpdated(const PropertyBase &prop)
	{
		if (prop == spritesheet)
		{
			SetSpritesheet(spritesheet);
		}
		else if (prop == animation)
		{
			SetAnimation(animation);
		}
		else if (prop == frame)
		{
			SetFrame(frame);
		}
	}
}