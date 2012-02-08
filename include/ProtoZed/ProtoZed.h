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
#ifndef PZ_ProtoZed_h__
#define PZ_ProtoZed_h__

#include <ProtoZed/Application.h>
#include <ProtoZed/Log.h>
#include <ProtoZed/Profiler.h>
#include <ProtoZed/Convert.h>
#include <ProtoZed/Helpers.h>
#include <ProtoZed/Vector2.h>
#include <ProtoZed/Angle.h>
#include <ProtoZed/Math.h>
#include <ProtoZed/UniqueIDGenerator.h>
#include <ProtoZed/Messages.h>

#include <ProtoZed/Animation/Animation.h>

#include <ProtoZed/MetaEntity.h>

#include <ProtoZed/Component.h>
#include <ProtoZed/Components/Position2D.h>
#include <ProtoZed/Components/Sprite.h>
//#include <ProtoZed/Components/SpritesheetComponent.h>
//#include <ProtoZed/Components/SoundComponent.h>

#include <ProtoZed/Service.h>
#include <ProtoZed/Services/Renderer.h>
#include <ProtoZed/Services/Input.h>

#include <ProtoZed/IncludeFilter.h>
#include <ProtoZed/IncludeFilters/NoFilter.h>
#include <ProtoZed/IncludeFilters/DistanceFilter.h>

#endif // PZ_ProtoZed_h__