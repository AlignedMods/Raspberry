project "Raspberry"
	language "C++"
	cppdialect "C++20"

    targetdir ( "../build/bin/" .. OutputDir .. "/%{prj.name}")
    objdir ( "../build/obj/" .. OutputDir .. "/%{prj.name}" )

	files { "**.cpp", "**.hpp" }

	includedirs { "../core/", "../vendor/raylib/src/", "../vendor/raygui/src/", "../vendor/json/include/", "../vendor/imgui/" }

    -- IMPORTANT: we must link core before raylib because ¯\_(ツ)_/¯
	links { "core", "raylib", "rlImGui", "imgui" }
	
	defines { "RAYMATH_DISABLE_CPP_OPERATORS" } -- honestly not a huge fan of those
	
	postbuildcommands { "{COPYFILE} %[%{cfg.targetdir}/../../../../rsp-src/core.rsp] %[%{cfg.targetdir}/core.rsp]"} -- copy the rsp file into the build directory so the game can be run

    filter "platforms:Windows"
        links { "winmm", "gdi32" }
