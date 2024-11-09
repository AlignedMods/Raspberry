@echo off

cd ..

cd vendor\raylib\

make -j -f Makefile-raylib

cd ..\..\

premake5 gmake2
make Raspberry

cd build\bin\Release\
start Raspberry.exe
