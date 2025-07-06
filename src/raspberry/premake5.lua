project "Raspberry"
	language "C++"
	cppdialect "C++23"
	location "../../build/"

	files { "**.cpp", "**.hpp" }

	includedirs { "../../vendor/raylib/src/" }

	filter "platforms:Windows"
		links { "raylib", "winmm", "gdi32" }

	filter "platforms:Linux"
		links { "raylib" }
