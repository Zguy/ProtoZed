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
#include <ProtoZed/Components/SpritesheetComponent.h>

#include <ProtoZed/Application.h>
#include <ProtoZed/Jzon.h>

namespace PZ
{
	SpritesheetComponent::SpritesheetComponent() : Component("Spritesheet"), animationIndex(0), currentFrame(0), currentTime(0.f)
	{

	}
	SpritesheetComponent::~SpritesheetComponent()
	{

	}

	void SpritesheetComponent::SetSpritesheet(const SpritesheetDefinition &spritesheet)
	{
		spritesheetDef = spritesheet;
		sprite.SetImage(Application::GetSingleton().GetImageStorage().GetAsset(spritesheetDef.image));
		SetAnimation(spritesheetDef.animations.at(0).name);
	}
	bool SpritesheetComponent::SetSpritesheetFromFile(const std::string &filename)
	{
		Jzon::NodePtr rootNode = Jzon::FileReader::ReadFile(filename);

		if (rootNode->IsValue() && rootNode->AsValue() == Jzon::null)
		{
			Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Error(Log::LVL_MEDIUM, "Entity \""+GetName()+"\" failed to open spritesheet \""+filename+"\"");
			return false;
		}
		else
		{
			Jzon::Object root = rootNode->AsObject();

			SpritesheetDefinition def;
			def.name   = root.Get("name").AsValue().AsString();
			def.image  = root.Get("image").AsValue().AsString();
			def.size.x = root.Get("width").AsValue().AsInt();
			def.size.y = root.Get("height").AsValue().AsInt();

			Jzon::Array &animationArray = root.Get("animations").AsArray();
			for (Jzon::Array::Iterator it = animationArray.Begin(); it != animationArray.End(); ++it)
			{
				Jzon::Object &animationObject = (*it).AsObject();
				SpritesheetAnimationDefinition aniDef;
				aniDef.name = animationObject.Get("name").AsValue().AsString();
				aniDef.from = animationObject.Get("start").AsValue().AsInt();
				aniDef.to   = animationObject.Get("end").AsValue().AsInt();
				aniDef.time = (float)animationObject.Get("time").AsValue().AsDouble();
				aniDef.loop = animationObject.Get("loop").AsValue().AsBool();

				def.animations.push_back(aniDef);
			}

			SetSpritesheet(def);

			return true;
		}
	}

	bool SpritesheetComponent::SetAnimation(const std::string &name)
	{
		bool found = false;
		for (unsigned int i = 0; i < spritesheetDef.animations.size(); ++i)
		{
			if (spritesheetDef.animations.at(i).name == name)
			{
				found = true;
				animationIndex = i;
				currentTime = 0.f;
				setFrame(0);
				break;
			}
		}

		if (!found)
		{
			Application::GetSingleton().GetLogManager().GetLog("ProtoZed").Error(Log::LVL_MEDIUM, "Entity \""+GetName()+"\" tried to set non-existent animation \""+name+"\"");
		}

		return found;
	}
	const std::string &SpritesheetComponent::GetAnimation() const
	{
		return spritesheetDef.animations.at(animationIndex).name;
	}

	bool SpritesheetComponent::HasAttribute(Attribute attribute) const
	{
		return ((attribute == Attributes::SCALE_X)||
						(attribute == Attributes::SCALE_Y)||
						(attribute == Attributes::COLOR_R)||
						(attribute == Attributes::COLOR_G)||
						(attribute == Attributes::COLOR_B)||
						(attribute == Attributes::ALPHA)||
						Component::HasAttribute(attribute));
	}
	void SpritesheetComponent::SetAttribute(Attribute attribute, float value)
	{
		Component::SetAttribute(attribute, value);

		if (attribute == Attributes::SCALE_X)
			SetScaleX(value);
		else if (attribute == Attributes::SCALE_Y)
			SetScaleY(value);
		else if ((attribute == Attributes::COLOR_R)||(attribute == Attributes::COLOR_G)||(attribute == Attributes::COLOR_B)||(attribute == Attributes::ALPHA))
		{
			sf::Color color = GetColor();
			if (attribute == Attributes::COLOR_R)
				color.r = static_cast<sf::Uint8>(value);
			else if (attribute == Attributes::COLOR_G)
				color.g = static_cast<sf::Uint8>(value);
			else if (attribute == Attributes::COLOR_B)
				color.b = static_cast<sf::Uint8>(value);
			else if (attribute == Attributes::ALPHA)
				color.a = static_cast<sf::Uint8>(value);
			SetColor(color);
		}
	}
	float SpritesheetComponent::GetAttribute(Attribute attribute) const
	{
		if (attribute == Attributes::SCALE_X)
			return GetScale().x;
		else if (attribute == Attributes::SCALE_Y)
			return GetScale().y;
		else if (attribute == Attributes::COLOR_R)
			return GetColor().r;
		else if (attribute == Attributes::COLOR_G)
			return GetColor().g;
		else if (attribute == Attributes::COLOR_B)
			return GetColor().b;
		else if (attribute == Attributes::ALPHA)
			return GetColor().a;
		else
			return Component::GetAttribute(attribute);
	}

	bool SpritesheetComponent::ReceiveMessage(Message &message)
	{
		if (message.message == MessageID::UPDATE)
		{
			UpdateMessage &updateMessage = message.As<UpdateMessage>();
			float deltaTime = updateMessage.deltaTime;

			currentTime += deltaTime;

			if (spritesheetDef.animations.size() > animationIndex)
			{
				if (currentTime >= (spritesheetDef.animations.at(animationIndex).time))
				{
					currentTime = 0.f;
					setFrame(currentFrame+1);
				}
			}

			return true;
		}
		else if (message.message == MessageID::POSITION_UPDATED)
		{
			sprite.SetPosition(GetOwner()->GetGlobalPosition());

			return true;
		}
		else if (message.message == MessageID::ROTATION_UPDATED)
		{
			sprite.SetRotation(GetOwner()->GetGlobalRotation());
			sprite.SetPosition(GetOwner()->GetGlobalPosition());

			return true;
		}
		else if (message.message == MessageID::DRAW)
		{
			DrawMessage &drawMessage = message.As<DrawMessage>();
			drawMessage.window.Draw(sprite);

			return true;
		}

		return false;
	}

	void SpritesheetComponent::setFrame(unsigned int frame)
	{
		SpritesheetAnimationDefinition &animation = spritesheetDef.animations.at(animationIndex);
		if (frame <= (animation.to - animation.from))
		{
			const unsigned int imageWidth = sprite.GetImage()->GetWidth();
			const unsigned int imageHeight = sprite.GetImage()->GetHeight();
			if (imageWidth > 0 && imageHeight > 0)
			{
				const unsigned int frameWidth = spritesheetDef.size.x;
				const unsigned int frameHeight = spritesheetDef.size.y;

				const unsigned int framesPerRow = imageWidth / frameWidth;
				const unsigned int actualFrame = animation.from+frame;

				const unsigned int frameY = (actualFrame/framesPerRow);

				const unsigned int x = (actualFrame-(frameY*framesPerRow))*frameWidth;
				const unsigned int y = frameY*frameHeight;
				const unsigned int w = frameWidth;
				const unsigned int h = frameHeight;
				sprite.SetSubRect(sf::IntRect(x,y,x+w,y+h));
				currentFrame = frame;
			}
		}
		else if (animation.loop)
		{
			setFrame(0);
		}
	}
}