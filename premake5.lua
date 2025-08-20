workspace "Raspberry"
	configurations { "Debug", "Release" }
	platforms { "Windows", "Linux" }

	includedirs { "raspberry/" }

    OutputDir = "%{cfg.buildcfg}-%{cfg.system}"

	filter "configurations:Release"
		optimize "On"
		kind "WindowedApp"

	filter "configurations:Debug"
		symbols "On"
		kind "ConsoleApp"
		defines { "RDEBUG" }

	-- submodules
    include "external.lua"

	include "blueberry/"
	include "raspberry/"
	include "rsp-src/"
	--require "src/shared"
