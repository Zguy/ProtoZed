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
#include <ProtoZed/Platform.h>
#include <ProtoZed/Log.h>
#include <ProtoZed/Profiler.h>
#include <ProtoZed/Convert.h>
#include <ProtoZed/Helpers.h>
#include <ProtoZed/Vector2.h>
#include <ProtoZed/Vector3.h>
#include <ProtoZed/Angle.h>
#include <ProtoZed/Math.h>
#include <ProtoZed/UniqueIDGenerator.h>
#include <ProtoZed/Path.h>
#include <ProtoZed/Random.h>

#include <ProtoZed/SystemList.h>
#include <ProtoZed/StateManager.h>
#include <ProtoZed/EntityManager.h>
#include <ProtoZed/AssetManager.h>

#include <ProtoZed/Animation/Animation.h>

#include <ProtoZed/MetaEntity.h>

#include <ProtoZed/Component.h>
#include <ProtoZed/Components/Position2D.h>
#include <ProtoZed/Components/Sprite.h>
#include <ProtoZed/Components/SoundEmitter.h>
#include <ProtoZed/Components/Spritesheet.h>

#include <ProtoZed/System.h>
#include <ProtoZed/Systems/Renderer.h>
#include <ProtoZed/Systems/Input.h>
#include <ProtoZed/Systems/Sound.h>

#include <ProtoZed/Archive.h>
#include <ProtoZed/Archives/FileSystemArchive.h>

#include <ProtoZed/Assets/ArchetypeAsset.h>
#include <ProtoZed/Assets/AssetGroupAsset.h>
#include <ProtoZed/Assets/ImageAsset.h>
#include <ProtoZed/Assets/SpritesheetAsset.h>

#include <ProtoZed/IncludeFilter.h>
#include <ProtoZed/IncludeFilters/NoFilter.h>
#include <ProtoZed/IncludeFilters/DistanceFilter.h>

#include <ProtoZed/Events/ComponentEvent.h>
#include <ProtoZed/Events/EntityEvent.h>
#include <ProtoZed/Events/LogEvent.h>
#include <ProtoZed/Events/MouseEvent.h>

#endif // PZ_ProtoZed_h__
