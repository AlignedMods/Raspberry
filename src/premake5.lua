project "Raspberry"
	language "C++"
	cppdialect "C++23"
	location "../build/"

	files { "**.cpp", "**.hpp" }
	removefiles { "blueberry/**.cpp" }

	includedirs { "../vendor/raylib/src/" }

	links { "raylib" }

	filter "platforms:Windows"
		links { "winmm", "gdi32" }
