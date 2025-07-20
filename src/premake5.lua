project "Raspberry"
	language "C++"
	cppdialect "C++20"
	location "../build/"

	files { "**.cpp", "**.hpp" }
	removefiles { "blueberry/**.cpp" }

	includedirs { "../vendor/raylib/src/", "../vendor/raygui/src/" }

	links { "raylib" }

	defines { "RAYMATH_DISABLE_CPP_OPERATORS" }

	filter "platforms:Windows"
		links { "winmm", "gdi32" }
