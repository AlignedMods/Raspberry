# Raspberry

| [What is it?](#what-is-it?) - [Building](#building) - [How it works](#how-it-works) - [What are archives?](#what-are-archives?) |
:----------------------------------------------------------: |

# What is it?
**Raspberry**  (as in the entire project) is *technically* not a game by itself, but instead:
 - [The game engine](https://github.com/AlignedMods/Raspberry/tree/main/core)
 - [The archive/map editor](https://github.com/AlignedMods/Raspberry/tree/main/blueberry)
 - [The game itself](https://github.com/AlignedMods/Raspberry/tree/main/raspberry)

### The game engine (Blackberry)
The game engine (named ***Blackberry***) is the essentially the core of the entire project.

The engine handles a lot of the boilerplate code that is required for things such as rendering, events and such,
It also handles **loading** and **serializing** [archives](#what-are-archives?).

### The archive/map editor (Blueberry)
The archive/map editor is an extremely important **tool** that can be used to modify/create [archives](#what-are-archives?) and maps.

### The game (Raspberry)
well... the game

# Building
Raspberry game uses the [premake](https://premake.github.io/) build system for cross-platform building.

**NOTE**: premake does **NOT** build the actual project, it only generates the build system!

If you wish to simply generate some project files, the simplest way to do (on **Windows** that is by running the command

    premake5 vs2022
This command will generate you visual studio project files and a Visual Studio solution that you can open in Visual Studio.

Note that you are not limited to only Visual Studio 2022, you can also generate project files for the following:

 - gmake (gmake2 on older versions or premake) - Creates Makefiles for compiling with make
 - vs2022 - Creates Visual Studio 2022 project files
 - vs2019 - Creates Visual Studio 2019 project files
 - vs2017 - Creates Visual Studio 2017 project files
 - vs2015 - Creates Visual Studio 2015 project files
 - vs2013 - Creates Visual Studio 2013 project files
 - vs2012 - Creates Visual Studio 2012 project files
 - vs2010 - Creates Visual Studio 2010 project files
 - xcode4 - Creates Apple Xcode 4 project files
 - codelite - Creates CodeLite project files

