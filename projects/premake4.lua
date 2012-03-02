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
	includedirs { "../include/", "C:/SDK/boost_1_48_0" }
	kind "StaticLib"
	
	configuration "Release"
		targetname "ProtoZed"
		
	configuration "Debug"
		targetname "ProtoZed_d"
		
--
-- Services
--
function DefineService(name, includes)

	project ("Service_"..name)
	defines { "PROFILER" }
	files { "../services/"..name.."/**.*" }
	includedirs { "../services/"..name.."/include/", "../include/", includes }
	kind "StaticLib"
	
	configuration "Release"
		targetname (name)
		
	configuration "Debug"
		targetname (name.."_d")

end

DefineService("Renderer_SFML", { "C:/SDK/SFML-1.6/include" })
DefineService("Input_SFML", { "C:/SDK/SFML-1.6/include", "../services/Renderer_SFML/include" })
