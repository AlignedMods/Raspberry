project "Raspberry"
	language "C++"
	cppdialect "C++20"
	location "../build/"

	files { "**.cpp", "**.hpp" }
	removefiles { "blueberry/**.cpp" }

	includedirs { "../vendor/raylib/src/", "../vendor/raygui/src/", "../vendor/rlImGui/", "../vendor/imgui/" }

	links { "raylib" }

	defines { "RAYMATH_DISABLE_CPP_OPERATORS" } -- honestly not a huge fan of those

	filter "platforms:Windows"
		links { "winmm", "gdi32" }
