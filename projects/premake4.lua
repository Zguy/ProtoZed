solution "ProtoZed"

	language "C++"
	location ( os.get() .. "/" .. _ACTION )
	flags { "Unicode", "NoPCH" }
	targetdir ( "../lib/" .. os.get() )
	libdirs { "../lib/", "../lib/" .. os.get() }

	configurations
	{ 
		"Release",
		"Debug"
	}
	
configuration "Release"
	defines { "NDEBUG" }
	
configuration "Debug"
	defines { "_DEBUG" }

--[[project "GWEN DLL"
	defines { "GWEN_COMPILE_DLL" }
	files { "../src/**.*", "../include/Gwen/**.*" }
	kind "SharedLib"
	
	configuration "Release"
		targetname( "gwen" )
		
	configuration "Debug"
		targetname( "gwend" )]]--

project "ProtoZed"
	defines { "PROFILER" }
	files { "../src/**.*", "../include/ProtoZed/**.*" }
	includedirs { "../include/" }
	--flags { "ExtraWarnings" }
	kind "StaticLib"
	
	configuration "Release"
		targetname( "ProtoZed" )
		flags { "Optimize" }
		
	configuration "Debug"
		targetname( "ProtoZed_d" )
		flags { "Symbols" }
		
--
-- Renderers
--

--[[DefineRenderer( "OpenGL", {"../Renderers/OpenGL/OpenGL.cpp"} )
DefineRenderer( "OpenGL_DebugFont", { "../Renderers/OpenGL/OpenGL.cpp", "../Renderers/OpenGL/DebugFont/OpenGL_DebugFont.cpp" } )
DefineRenderer( "SFML", { "../Renderers/SFML/SFML.cpp" } )
DefineRenderer( "Allegro", { "../Renderers/Allegro/Allegro.cpp" } )

if ( os.get() == "windows" ) then
	DefineRenderer( "DirectX9", { "../Renderers/DirectX9/DirectX9.cpp" } )
	DefineRenderer( "Direct2D", { "../Renderers/Direct2D/Direct2D.cpp" } )
	DefineRenderer( "GDI", { "../Renderers/GDIPlus/GDIPlus.cpp", "../Renderers/GDIPlus/GDIPlusBuffered.cpp" } )
end]]--