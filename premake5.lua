workspace("Raspberry")
configurations({ "Release", "Debug" })
platforms({ "Windows", "Linux" })

filter("configurations:Release")
optimize("On")
architecture("x86_64")

kind("WindowedApp")
language("C++")
cppdialect("C++20")

-- linking with raylib
libdirs({ "vendor/raylib/src/" })
links({ "raylib" })

filter("configurations:Debug")
defines({ "RDEBUG", "RTRACEASSETS" })
language("C++")
cppdialect("C++20")
symbols("On")
architecture("x86_64")

kind("ConsoleApp")

-- linking with raylib
libdirs({ "vendor/raylib/src/" })
links({ "raylib" })

-- Windows requires some extra dependencies to compile with raylib
filter("platforms:Windows")
links({ "winmm", "gdi32" })

-- for building the actual game
project("Raspberry")
location("build/")
files({ "src/**.cpp", "src/**.hpp" })
removefiles({ "src/builder/**.cpp", "src/builder/**.hpp" })

includedirs({ "vendor/raylib/src/", "vendor/raylib-cpp/include/", "src" })

pchheader("pch.hpp")

-- I hate visual studio
pchsource("src/pchsource.cpp")

-- for bulding the map builder
project("Raspberry-Builder")
location("build/")
files({
	"src/builder/**.cpp",
	"src/builder/**.hpp",
	"src/tile/tile.cpp",
	"src/tile/tile.hpp",
	"src/utils/types.cpp",
	--"src/core/tinyfiledialogs.cpp",
	--"src/core/tinyfiledialogs.h",
})

includedirs({ "vendor/raylib/src/", "vendor/raylib-cpp/include/", "src", "vendor/raygui/src/" })

pchheader("pch.hpp")

-- Necessary for certain things like the tile system
defines({ "BUILDER" })
