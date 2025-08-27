project "core"
    language "C++"
    cppdialect "C++20"
    kind "StaticLib"

    targetdir ( "../build/bin/" .. OutputDir .. "/%{prj.name}")
    objdir ( "../build/obj/" .. OutputDir .. "/%{prj.name}" )

    files { "**.cpp", "**.hpp" }

    includedirs { "../core/", "../vendor/raylib/src/", "../vendor/rlImGui/", "../vendor/json/include/" }

    links { "raylib", "rlImGui" }

    filter "platforms:Windows"
        links { "winmm", "gdi32" }