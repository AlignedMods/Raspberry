workspace "Raspberry"
	configurations {"Release", "Debug"}

	filter "configurations:Release"
		optimize "On"
		architecture "x86_64"

	filter "configurations:Debug"
		defines {"R_DEBUG"}
		symbols "On"
		architecture "x86_64"

	-- for building the actual game
	project "Raspberry"
		kind "ConsoleApp"
		language "C++"
		includedirs {"include", "src"}
		location "build/"
		files {"src/**.cpp"}
		removefiles {"src/builder/**.cpp"}

		-- linking with raylib
		libdirs {"libs"}
		links {"raylibdll"}

	-- for bulding the map builder
	project "Raspberry-Builder"
		kind "ConsoleApp"
		language "C++"
		includedirs {"include", "src"}
		location "build/"
		files {"src/builder/**.cpp", "src/tile/tile.cpp"}

		-- Necessary for certain things like the tile system
		defines {"BUILDER"}

		--linking with raylib
		libdirs {"libs"}
		links {"raylibdll"}
		
