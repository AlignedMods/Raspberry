# Raspberry

## How to build:

### **1: Prerequisites**

***A C++ compiler (duh)***

***Premake***

***Make*** or ***Visual Studio***

### **2: Building the source code**

To generate the build system you wish run:

    $ premake5 gmake (or vs2022)
    $ make config=debug_windows (change this to debug_linux if building on linux, change debug to release if building for release)

(The executable will be put in *build/bin/configuration-platform/Raspberry/*)
