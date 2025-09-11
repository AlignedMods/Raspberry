workspace "Raspberry" -- The entire project
    configurations { "Debug", "Release" }
    platforms { "x86", "x64" }

    OutputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    defines { "_CRT_SECURE_NO_WARNINGS" }

    filter "configurations:Release"
        optimize "On"

    filter "configurations:Debug"
        symbols "On"
        defines { "RDEBUG" }

    filter "platforms:x64"
        architecture "x64"

    filter "platforms:x86"
        architecture "x86"

    -- submodules
    include "external.lua"

    include "core/"
    include "blueberry/"
    include "raspberry/"