# Raspberry

## How to build:

### **1: Prerequisites**

***Premake***

***Make*** or ***Visual Studio***

***Raylib*** (only dynamic for now, for now the code only works on windows and the library is included in the libs folder)

### **2: Building the source code**

To generate the build system you wish run:

    premake5 gmake2
(for make)

**OR**

    premake5 vs2022
(for visual studio)

And after that run:

    make Raspberry // for building raspberry with make
    ./Raspberry.sln // for opening the project in visual studio

### **3: Running the game**

**You will probably have to copy the Assets and libs/raylib.dll into the build folder or directory you are running the game from**

    ./build/bin/Release/Raspberry.exe