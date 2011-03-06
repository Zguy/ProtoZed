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
#include <ProtoZed/Entities/SpritesheetEntity.h>

#include <ProtoZed/Entities/DrawableEntity.h>
#include <ProtoZed/Application.h>

namespace PZ
{
	SpritesheetEntity::SpritesheetEntity(const std::string &name) : Entity(name), animationIndex(0), currentFrame(0), currentTime(0.f)
	{

	}
	SpritesheetEntity::~SpritesheetEntity()
	{

	}

	void SpritesheetEntity::SetSpritesheet(const SpritesheetDefinition &spritesheet)
	{
		spritesheetDef = spritesheet;
		sprite.SetImage(PZ::Application::GetSingleton().GetImageStorage().GetAsset(spritesheetDef.image));
		SetAnimation(spritesheetDef.animations.at(0).name);
	}
	bool SpritesheetEntity::SetSpritesheetFromFile(const std::string &filename)
	{
		// Not implemented yet
		return false;
	}

	bool SpritesheetEntity::SetAnimation(const std::string &name)
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
	const std::string &SpritesheetEntity::GetAnimation() const
	{
		return spritesheetDef.animations.at(animationIndex).name;
	}

	bool SpritesheetEntity::HasAttribute(Attribute attribute)
	{
		return ((attribute == Attributes::X)||
						(attribute == Attributes::Y)||
						(attribute == Attributes::SCALE_X)||
						(attribute == Attributes::SCALE_Y)||
						(attribute == Attributes::ROTATION)||
						(attribute == Attributes::COLOR_R)||
						(attribute == Attributes::COLOR_G)||
						(attribute == Attributes::COLOR_B)||
						(attribute == Attributes::ALPHA));
	}
	void SpritesheetEntity::SetAttribute(Attribute attribute, float value)
	{
		if (attribute == Attributes::X)
			SetX(value);
		else if (attribute == Attributes::Y)
			SetY(value);
		else if (attribute == Attributes::SCALE_X)
			SetScaleX(value);
		else if (attribute == Attributes::SCALE_Y)
			SetScaleY(value);
		else if (attribute == Attributes::ROTATION)
			SetLocalRotation(value);
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
	float SpritesheetEntity::GetAttribute(Attribute attribute) const
	{
		if (attribute == Attributes::X)
			return GetLocalPosition().x;
		else if (attribute == Attributes::Y)
			return GetLocalPosition().y;
		else if (attribute == Attributes::SCALE_X)
			return GetScale().x;
		else if (attribute == Attributes::SCALE_Y)
			return GetScale().y;
		else if (attribute == Attributes::ROTATION)
			return GetLocalRotation();
		else if (attribute == Attributes::COLOR_R)
			return GetColor().r;
		else if (attribute == Attributes::COLOR_G)
			return GetColor().g;
		else if (attribute == Attributes::COLOR_B)
			return GetColor().b;
		else if (attribute == Attributes::ALPHA)
			return GetColor().a;
		else
			return 0.f;
	}

	bool SpritesheetEntity::OnMessage(MessagePtr message)
	{
		bool handled = Entity::OnMessage(message);

		if (message->message == MessageID::UPDATE)
		{
			UpdateMessage *updateMessage = static_cast<UpdateMessage*>(message.get());
			float deltaTime = updateMessage->deltaTime;

			currentTime += deltaTime;

			if (currentTime >= (spritesheetDef.animations.at(animationIndex).time))
			{
				currentTime = 0.f;
				setFrame(currentFrame+1);
			}

			return true;
		}
		else if (message->message == MessageID::POSITION_UPDATED)
		{
			sprite.SetPosition(GetGlobalPosition());
			sprite.SetRotation(GetGlobalRotation());

			return true;
		}
		else if (message->message == MessageID::DRAW)
		{
			DrawMessage *drawMessage = static_cast<DrawMessage*>(message.get());
			drawMessage->window.Draw(sprite);

			return true;
		}

		return handled;
	}

	void SpritesheetEntity::setFrame(unsigned int frame)
	{
		SpritesheetAnimationDefinition &animation = spritesheetDef.animations.at(animationIndex);
		if (frame < (animation.to - animation.from))
		{
			const unsigned int imageWidth = sprite.GetImage()->GetWidth();
			const unsigned int imageHeight = sprite.GetImage()->GetHeight();
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
		else if (animation.loop)
		{
			setFrame(0);
		}
	}
}