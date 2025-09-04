project "raylib"
    language "C"
    cdialect "C99"
    kind "StaticLib"

    targetdir("build/bin/" .. OutputDir .. "/%{prj.name}")
    objdir("build/obj/" .. OutputDir .. "/%{prj.name}")

    files { "vendor/raylib/src/*.c", "vendor/raylib/src/*.h" }

    includedirs { "vendor/raylib/src", "vendor/raylib/src/external/glfw/include" }

    defines { "SUPPRESS_DEF_PLATFORM", "PLATFORM_DESKTOP", "GRAPHICS_API_OPENGL_33", "SUPPORT_CUSTOM_FRAME_CONTROL", "SUPPORT_MODULE_RAUDIO" }

    filter "platforms:Windows"
        links { "opengl32", "shell32", "gdi32", "winmm" }

    filter "platforms:Linux"
        defines { "_GLFW_X11" }
        links { "GL", "X11", "m", "pthread", "dl" }

project "imgui"
    language "C++"
    cppdialect "C++20"
    kind "StaticLib"

    targetdir("build/bin/" .. OutputDir .. "/%{prj.name}")
    objdir("build/obj/" .. OutputDir .. "/%{prj.name}")

    files { "vendor/imgui/*.cpp", "vendor/imgui/*.hpp" }

    includedirs { "vendor/imgui/" }

project "rlImGui"
    language "C++"
    cppdialect "C++20"
    kind "StaticLib"

    targetdir("build/bin/" .. OutputDir .. "/%{prj.name}")
    objdir("build/obj/" .. OutputDir .. "/%{prj.name}")

    files { "vendor/rlImGui/*.cpp", "vendor/rlImGui/*.hpp" }

    includedirs { "vendor/rlImGui/", "vendor/imgui/", "vendor/raylib/src/" }

    links { "imgui" }

    configurations { "platform:Windows" }
        links { "winmm", "gdi32" }
