project "Blackberry" -- The game engine
    language "C++"
    cppdialect "C++20"
    kind "StaticLib"

    targetdir ( "../build/bin/" .. OutputDir .. "/%{prj.name}")
    objdir ( "../build/obj/" .. OutputDir .. "/%{prj.name}" )

    files { "**.cpp", "**.hpp" }

    includedirs { "../core/", "../vendor/glfw/include/", "../vendor/imgui/", "../vendor/glad/include/", "../vendor/glm", "../vendor/stb/" }

    links { "glfw", "glad", "imgui" }