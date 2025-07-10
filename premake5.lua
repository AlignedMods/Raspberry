workspace "Raspberry"
	configurations { "Debug", "Release" }
	platforms { "Windows", "Linux" }

	includedirs { "src/" }

	filter "configurations:Release"
		optimize "On"
		kind "WindowedApp"

	filter "configurations:Debug"
		symbols "On"
		kind "ConsoleApp"
		defines { "RDEBUG" }

	project "raylib"
		language "C"
		cdialect "C99"
		kind "StaticLib"
		location "build/"

		files { "vendor/raylib/src/*.c", "vendor/raylib/src/*.h" }

		includedirs { "vendor/raylib/src", "vendor/raylib/src/external/glfw/include" }

		defines { "SUPPRESS_DEF_PLATFORM", "PLATFORM_DESKTOP", "GRAPHICS_API_OPENGL_33", "SUPPORT_CUSTOM_FRAME_CONTROL" }

		filter "platforms:Windows"
			links { "opengl32", "shell32", "gdi32", "winmm" }

		filter "platforms:Linux"
			defines { "_GLFW_X11" }
			links { "GL", "X11", "m", "pthread", "dl" }

	-- submodules
	--include "src/blueberry"
	include "src/"
	--require "src/shared"
