dofile( "bubblewrap/Scripts/Bubblewrap.lua" )
dofile ( "bubblewrap/Scripts/Bubblewrap_Scripts.lua" )

-- Solution.
PsySolutionGame( "LudumDare32" )


-- Build externals.
dofile ("bubblewrap/External/genie.lua")

loadLibrary( "bubblewrap_bgfx" )

-- Build engine.
dofile ("bubblewrap/bubblewrap_genie.lua")

-- Build game source.
dofile ("./Source/genie.lua")

