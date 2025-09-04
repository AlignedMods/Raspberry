project "Blueberry"
	language "C++"
	cppdialect "C++20"
    kind "ConsoleApp"

    targetdir ( "../build/bin/" .. OutputDir .. "/%{prj.name}")
    objdir ( "../build/obj/" .. OutputDir .. "/%{prj.name}" )

	files { "**.cpp", "**.hpp" }

	includedirs { "../core", "../vendor/raylib/src/", "../vendor/imgui/", "../vendor/rlImGui/", "./" }

	links { "core", "raylib", "rlImGui", "imgui" }

	defines { "RAYMATH_DISABLE_CPP_OPERATORS" } -- honestly not a huge fan of those

    filter "platforms:Windows"
        links { "winmm", "gdi32" }
