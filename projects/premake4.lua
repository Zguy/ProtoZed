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
	flags { "Optimize" }
	
configuration "Debug"
	defines { "_DEBUG" }
	flags { "Symbols" }

project "ProtoZed"
	defines { "PROFILER" }
	files { "../src/**.*", "../include/ProtoZed/**.*" }
	includedirs { "../include/", "C:/SDK/SFML-1.6/include", "C:/SDK/boost_1_48_0" }
	--flags { "ExtraWarnings" }
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
DefineService("Input_SFML", { "C:/SDK/SFML-1.6/include" })
