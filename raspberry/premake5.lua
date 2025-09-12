project "Raspberry" -- the actual game
    language "C++"
    cppdialect "C++20"
    kind "ConsoleApp"

    targetdir ( "../build/bin/" .. OutputDir .. "/%{prj.name}")
    objdir ( "../build/obj/" .. OutputDir .. "/%{prj.name}" )

    files { "**.cpp", "**.hpp" }

    includedirs { "../core/", "../vendor/json/include/", "../vendor/imgui/", "./" }

    links { "Blackberry" }
    links { "glfw", "opengl32", "imgui" }
	
    postbuildcommands { "{COPYFILE} %[%{cfg.targetdir}/../../../../rsp-src/core.rsp] %[%{cfg.targetdir}/core.rsp]"} -- copy the rsp file into the build directory so the game can be run
