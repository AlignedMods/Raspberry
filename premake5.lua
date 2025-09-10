workspace "Raspberry" -- The entire project
    configurations { "Debug", "Release" }

    OutputDir = "%{cfg.buildcfg}-%{cfg.system}"

    defines { "_CRT_SECURE_NO_WARNINGS" }

    filter "configurations:Release"
        optimize "On"

    filter "configurations:Debug"
        symbols "On"
        defines { "RDEBUG" }

    -- submodules
    include "external.lua"

    include "core/"
    include "blueberry/"
    include "raspberry/"