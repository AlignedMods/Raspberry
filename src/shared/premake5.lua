project "Raspberry-Shared"
	language "C++"
	cppdialect "C++23"
	location "../../build/"
	kind "StaticLib"

	files { "**.cpp", "**.hpp" }

	links { "raylib" }
