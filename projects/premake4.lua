solution "ProtoZed"

	language "C++"
	location ( os.get() .. "/" .. _ACTION )
	flags { "ExtraWarnings", "Unicode", "NoPCH" }
	targetdir ( "../lib/" .. os.get() )

	configurations
	{ 
		"Release",
		"Debug"
	}
	
configuration "linux"
	buildoptions { "-std=c++0x" }
	
configuration "Release"
	defines { "NDEBUG" }
	flags { "Optimize" }
	
configuration "Debug"
	defines { "_DEBUG" }
	flags { "Symbols" }

project "ProtoZed"
	defines { "PROFILER" }
	files { "../src/**.*", "../include/ProtoZed/**.*" }
	includedirs { "../include/" }
	kind "StaticLib"
	
	configuration "Release"
		targetname "ProtoZed"
		
	configuration "Debug"
		targetname "ProtoZed_d"
		
--
-- Systems
--
function DefineSystem(name, includes)

	project ("System_"..name)
	defines { "PROFILER" }
	files { "../systems/"..name.."/**.*" }
	includedirs { "../systems/"..name.."/include/", "../include/", includes }
	kind "StaticLib"
	
	configuration "Release"
		targetname (name)
		
	configuration "Debug"
		targetname (name.."_d")

end

DefineSystem("Renderer_SFML", { "C:/SDK/SFML-1.6/include" })
DefineSystem("Input_SFML", { "C:/SDK/SFML-1.6/include", "../systems/Renderer_SFML/include" })
DefineSystem("Sound_SFML", { "C:/SDK/SFML-1.6/include" })
