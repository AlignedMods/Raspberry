project "Blueberry"
	language "C++"
	cppdialect "C++23"
	location "../../build/"

	files { "**.cpp", "**.hpp" }

	includedirs { "../../vendor/raylib/src/", "../../vendor/raygui/src/" }

	defines { "BUILDER" }

	filter "platforms:Windows"
		links { "raylib", "winmm", "gdi32" }

	filter "platforms:Linux"
		links { "raylib" }
