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

function SetPlatform(name)

	local platforms = os.matchfiles("../src/Platforms/*.cpp")
	
	for i, file in ipairs(platforms) do
		local filename = path.getbasename(file)
		if filename == name then
			files { file }
		else
			excludes { file }
		end
	end
	
end
	
project "ProtoZed"
	kind "StaticLib"
	includedirs { "../include/" }

	files { "../src/**.*", "../include/ProtoZed/**.*" }
	
	configuration "windows"
		SetPlatform "Windows"
	configuration "linux"
		SetPlatform "POSIX"
	
	configuration "Release"
		targetname "ProtoZed"
		
	configuration "Debug"
		targetname "ProtoZed_d"
		
--
-- Systems
--
function DefineSystem(name, includes)

	project ("System_"..name)
		kind "StaticLib"
		includedirs { "../systems/"..name.."/include/", "../include/", includes }
		
		files { "../systems/"..name.."/**.*" }
		
		configuration "Release"
			targetname (name)
			
		configuration "Debug"
			targetname (name.."_d")

end

DefineSystem("Renderer_SFML", { "C:/SDK/SFML-1.6/include" })
DefineSystem("Input_SFML", { "C:/SDK/SFML-1.6/include", "../systems/Renderer_SFML/include" })
DefineSystem("Sound_SFML", { "C:/SDK/SFML-1.6/include" })
