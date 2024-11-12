-- some wierd ass auto formatting is going on here

workspace("Raspberry")
configurations({ "Release", "Debug" })

filter("configurations:Release")
optimize("On")
architecture("x86_64")

kind("WindowedApp")
language("C++")
cppdialect("C++20")
includedirs({ "include", "src" })

-- linking with raylib
libdirs({ "libs" })
links({ "raylib", "gdi32", "winmm" })

filter("configurations:Debug")
defines({ "RDEBUG" })
symbols("On")
architecture("x86_64")

kind("ConsoleApp")
language("C++")
includedirs({ "include", "src" })

-- linking with raylib
libdirs({ "libs" })
links({ "raylib", "gdi32", "winmm" })

-- for building the actual game
project("Raspberry")
location("build/")
files({ "src/**.cpp", "src/**.hpp" })
removefiles({ "src/builder/**.cpp", "src/builder/**.hpp" })

pchheader("Raspberry.hpp")

-- I hate visual studio
pchsource("src/pchsource.cpp")

-- for bulding the map builder
project("Raspberry-Builder")
location("build/")
files({ "src/builder/**.cpp", "src/builder/**.hpp", "src/tile/tile.cpp", "src/tile/tile.hpp" })

pchheader("pch.hpp")

-- Necessary for certain things like the tile system
defines({ "BUILDER" })
