/*
Copyright (c) 2013 Johannes H�ggqvist

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
#include <ProtoZed/Components/Sprite.h>

#include <ProtoZed/Application.h>
#include <ProtoZed/AssetLibrary.h>
#include <ProtoZed/Assets/ImageAsset.h>

namespace PZ
{
	const HashString Sprite::Family = "Sprite";

	Sprite::Sprite() : image("Image"), center("Center"), color("Color"), regionOffset("RegionOffset"), regionSize("RegionSize")
	{
		AddProperty(image);
		AddProperty(center);
		AddProperty(color);
		AddProperty(regionOffset);
		AddProperty(regionSize);
	}
	Sprite::~Sprite()
	{
	}

	const ImageAsset *Sprite::GetImageAsset() const
	{
		AssetLibrary &assetLibrary = GetApplication().GetAssetLibrary();
		const ImageAsset *asset = assetLibrary.Get<ImageAsset>(GetImage());
		return asset;
	}
}