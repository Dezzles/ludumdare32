
PsyProjectGameExe( solution().name )
	files { "./**.h", "./**.hpp", "./**.cpp", "./**.h" }
	debugdir "../Dist"
	local allIncludes = 
	{
		"../bubblewrap/include",
		"../bubblewrap/External",
	}
	for k, v in pairs( additionalIncludes ) do
		table.insert(allIncludes, v)
	end 
	includedirs 
	{
		allIncludes
	}
	-- External includes.
	includedirs { 
      "../Psybrus/External/jsoncpp/include", 
   }

   PsyAddEngineLinks {
      "base",
   }
   
   PsyAddExternalLinks {
      "physfs",
      "json",
   }
	for k, v in pairs( additionalExternalLibraries ) do
		PsyAddExternalLinks { v }
	end 

	for k, v in pairs( additionalOtherLibraries ) do
		PsyAddOtherLinks { v }
	end 

	PsyAddEngineLinks ( additionalLibraries ) 
	
