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
#ifndef PZ_Position2D_h__
#define PZ_Position2D_h__

#include <ProtoZed/Component.h>

#include <ProtoZed/Vector2.h>
#include <ProtoZed/Angle.h>
#include <ProtoZed/Components/SceneNode.h>

namespace PZ
{
	typedef Vector2<Vector2f> Axes;

	class Position2D : public Component
	{
	public:
		static const HashString Family;

		Position2D(const EntityID &owner, EntityManager &manager) : Component(owner, manager), pos(0.f,0.f), depth(0.f), rotation(0.f), axes(globalAxes), parentAxesCache(globalAxes), inheritAxes(true), inheritPosition(true), inheritRotation(true)
		{}
		~Position2D()
		{}
	
		const Vector2f GetPosition(Scope::Level scope = Scope::LOCAL) const;
		void SetPosition(const Vector2f &newPos, Scope::Level scope = Scope::LOCAL);
		inline void SetPosition(float newX, float newY, Scope::Level scope = Scope::LOCAL)
		{
			SetPosition(Vector2f(newX, newY), scope);
		}
		inline void SetX(float newX, Scope::Level scope = Scope::LOCAL)
		{
			SetPosition(Vector2f(newX, GetPosition(scope).y), scope);
		}
		inline void SetY(float newY, Scope::Level scope = Scope::LOCAL)
		{
			SetPosition(Vector2f(GetPosition(scope).x, newY), scope);
		}
		inline void Move(const Vector2f &dPos, Scope::Level scope = Scope::LOCAL)
		{
			SetPosition(GetPosition(scope)+dPos, scope);
		}
		inline void Move(float dX, float dY, Scope::Level scope = Scope::LOCAL)
		{
			Move(Vector2f(dX,dY), scope);
		}

		inline float GetDepth() const
		{
			return depth;
		}
		inline void SetDepth(float newDepth)
		{
			depth = newDepth;
		}
		inline void MoveDepth(float dDepth)
		{
			depth += dDepth;
		}

		Angle::Degrees GetRotation(Scope::Level scope = Scope::LOCAL) const;
		void SetRotation(Angle::Degrees newRotation, Scope::Level scope = Scope::LOCAL);
		inline void Rotate(Angle::Degrees angle, Scope::Level scope = Scope::LOCAL)
		{
			SetRotation(GetRotation(scope)+angle, scope);
		}

		bool GetInheritAxes() const
		{
			return inheritAxes;
		}
		void SetInheritAxes(bool inherit)
		{
			inheritAxes = inherit;
		}
		bool GetInheritPosition() const
		{
			return inheritPosition;
		}
		void SetInheritPosition(bool inherit)
		{
			inheritPosition = inherit;
		}
		bool GetInheritRotation() const
		{
			return inheritRotation;
		}
		void SetInheritRotation(bool inherit)
		{
			inheritRotation = inherit;
		}

		Vector2f ConvertGlobalToLocal(const Vector2f &position) const;
		Vector2f ConvertLocalToGlobal(const Vector2f &position) const;

		virtual bool HandleMessage(const Message &message);

	private:
		void updateAxes();
		void updateChildren();

		Vector2f pos;
		float depth;
		Angle::Degrees rotation;

		Axes axes;

		Axes parentAxesCache;
		Vector2f parentPosCache;
		Angle::Degrees parentRotationCache;

		bool inheritAxes;
		bool inheritPosition;
		bool inheritRotation;

		static const Axes globalAxes;
	};
}

#endif // PZ_Position2D_h__