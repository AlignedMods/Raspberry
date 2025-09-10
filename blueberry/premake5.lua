project "Blueberry" -- the editor for the game
    language "C++"
    cppdialect "C++20"
    kind "ConsoleApp"

    targetdir ( "../build/bin/" .. OutputDir .. "/%{prj.name}")
    objdir ( "../build/obj/" .. OutputDir .. "/%{prj.name}" )

    files { "**.cpp", "**.hpp" }

    includedirs { "../core" }

    links { "Blackberry" }
    links { "glfw", "glad", "imgui" }
    