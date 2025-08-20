project "Raspberry"
	language "C++"
	cppdialect "C++20"
    kind "WindowedApp"

    targetdir ( "../build/bin/" .. OutputDir .. "/%{prj.name}")
    objdir ( "../build/obj/" .. OutputDir .. "/%{prj.name}" )

    ArchiverArgs = "../rsp-src/Assets/ " .. "%{cfg.targetdir}/core.rsp"

	files { "**.cpp", "**.hpp" }

	includedirs { "../shared/", "../vendor/raylib/src/", "../vendor/raygui/src/", "../vendor/json/include/" }

	links { "raylib" }

	defines { "RAYMATH_DISABLE_CPP_OPERATORS" } -- honestly not a huge fan of those

    filter "platforms:Windows"
        links { "winmm", "gdi32" }
