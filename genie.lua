dofile( "params-" .. _ACTION .. ".lua" )
additionalIncludes = {}
additionalLibraries = {}

dofile ( "bubblewrap/Scripts/Bubblewrap_Scripts.lua" )

solution "LudumDare32"
	configurations { "Debug", "Release", "CrashNBurn" }
	location "build"


-- Build externals.
dofile ("bubblewrap/External/genie.lua")

loadLibrary( "bubblewrap_sfml" )

-- Build engine.
dofile ("bubblewrap/bubblewrap_genie.lua")

-- Build game source.
dofile ("./Source/genie.lua")

