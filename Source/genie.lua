project "LudumDare32"
	kind "ConsoleApp"
	language "C++"
	files { "**.h", "**.hpp", "**.cpp" }
	location ( "../build" )
	targetdir ( "../build/out/" )
	objdir ( "../build/obj" )
	local allIncludes = 
	{
		"../bubblewrap/include",
		"../bubblewrap/External",
		SFML2DIR .. "/include/"	
	}
	for k, v in pairs( additionalIncludes ) do
		table.insert(allIncludes, v)
	end 
	includedirs 
	{
		allIncludes
	}
	links
	{
		"Bubblewrap", 
		"External_Json",
		additionalLibraries

	}
	
	buildoptions "-std=c++11"
	configuration "CrashNBurn"
		targetdir ( "../build/lib/crashnburn" )
		defines { "DEBUG", "CRASHNBURN" }
		flags { "Symbols" }
		links
		{
			SFML2DIR .. "/lib/sfml-graphics-d",
			SFML2DIR .. "/lib/sfml-window-d",
			SFML2DIR .. "/lib/sfml-system-d",
			SFML2DIR .. "/lib/sfml-audio-d",
			PHYSFSBUILDDIR .. "/Debug/physfs"
		
		}
		libdirs
		{
			"../build/lib/debug"
		}


	configuration "Debug"
		defines { "DEBUG" }
		flags { "Symbols" }
		links
		{
			SFML2DIR .. "/lib/sfml-graphics-d",
			SFML2DIR .. "/lib/sfml-window-d",
			SFML2DIR .. "/lib/sfml-system-d",
			SFML2DIR .. "/lib/sfml-audio-d",
			PHYSFSBUILDDIR .. "/Release/physfs"
		}
		libdirs
		{
			"../build/lib/debug"
		}

	configuration "Release"
		defines { "NDEBUG" }
		flags { "Optimize" }
		links
		{
			SFML2DIR .. "/lib/sfml-graphics",
			SFML2DIR .. "/lib/sfml-window",
			SFML2DIR .. "/lib/sfml-system",
			SFML2DIR .. "/lib/sfml-audio"
		
		}
		libdirs
		{
			"../build/lib/release"
		}
