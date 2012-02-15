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
#include <ProtoZed/Components/Position2D.h>

#include <ProtoZed/Messages.h>

namespace PZ
{
	const HashString Position2D::Family = "Position";

	const Axes Position2D::globalAxes(Vector2f(1.f,0.f), Vector2f(0.f,1.f));

	const Vector2f Position2D::GetPosition(Scope::Level scope) const
	{
		Vector2f finalPos;

		switch (scope)
		{
		case Scope::GLOBAL : finalPos = ConvertLocalToGlobal(pos); break;
		case Scope::LOCAL  : finalPos = pos;                       break;
		default            : finalPos = pos;                       break;
		}
		
		return finalPos;
	}
	void Position2D::SetPosition(const Vector2f &newPos, Scope::Level scope)
	{
		if (scope == Scope::GLOBAL)
		{
			pos = ConvertGlobalToLocal(newPos);
		}
		else if (scope == Scope::LOCAL)
		{
			pos = newPos;
		}

		updateChildren();
	}

	Angle::Degrees Position2D::GetRotation(Scope::Level scope) const
	{
		if (scope == Scope::LOCAL)
		{
			return rotation;
		}
		else if (scope == Scope::GLOBAL)
		{
			if (inheritRotation)
				return parentRotationCache + rotation;
			else
				return rotation;
		}
		else
		{
			return rotation;
		}
	}
	void Position2D::SetRotation(Angle::Degrees newRotation, Scope::Level scope)
	{
		if (scope == Scope::LOCAL)
		{
			rotation = newRotation;
		}
		else if (scope == Scope::GLOBAL)
		{
			if (inheritRotation)
				rotation = newRotation - parentRotationCache;
			else
				rotation = newRotation;
		}

		updateAxes();
	}

	Vector2f Position2D::ConvertGlobalToLocal(const Vector2f &position) const
	{
		Axes _axes = globalAxes;

		if (inheritAxes)
		{
			Angle::Radians angle = parentRotationCache;
			_axes.x.x = std::cos(angle.ToFloat());
			_axes.x.y = std::sin(angle.ToFloat());
			_axes.y.x = -_axes.x.y;
			_axes.y.y = _axes.x.x;
		}

		Vector2f aaPos = position;
		if (inheritPosition)
			aaPos -= parentPosCache;

		const Vector2f _pos = aaPos.x * _axes.x + aaPos.y * _axes.y;

		return _pos;
	}
	Vector2f Position2D::ConvertLocalToGlobal(const Vector2f &position) const
	{
		const Axes &_axes = (inheritAxes ? parentAxesCache : globalAxes);

		Vector2f _pos = position.x * _axes.x + position.y * _axes.y;
		if (inheritPosition)
			_pos += parentPosCache;

		return _pos;
	}

	bool Position2D::HandleMessage(const Message &message)
	{
		if (message.type == Messages::CHILD)
		{
			ChildMessage childMessage(message);
			if (childMessage.added)
			{
				updateChildren();
			}

			return true;
		}

		return false;
	}

	void Position2D::updateAxes()
	{
		Angle::Radians angle = -GetRotation(Scope::GLOBAL);
		axes.x.x = std::cos(angle.ToFloat());
		axes.x.y = std::sin(angle.ToFloat());
		axes.y.x = -axes.x.y;
		axes.y.y = axes.x.x;

		updateChildren();
	}
	void Position2D::updateChildren()
	{
		SceneNode *node = GetManager().GetComponent<SceneNode>(GetOwnerID());
		if (node != nullptr)
		{
			const EntityList &children = node->GetChildren();
			for (EntityList::const_iterator it = children.cbegin(); it != children.cend(); ++it)
			{
				Position2D *childPosition = GetManager().GetComponent<Position2D>(*it);
				if (childPosition != nullptr)
				{
					childPosition->parentAxesCache = axes;
					childPosition->parentPosCache = parentPosCache+pos;
					childPosition->parentRotationCache = parentRotationCache+rotation;
				}
			}
		}
	}
}