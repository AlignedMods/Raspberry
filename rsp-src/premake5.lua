project "rsp-archiver"
    language "C++"
    cppdialect "C++20"
    kind "ConsoleApp"

    targetdir ("../build/bin/" .. OutputDir .. "/%{prj.name}")
    objdir ("../build/obj/" .. OutputDir .. "/%{prj.name}")

    files { "archiver.cpp" }
