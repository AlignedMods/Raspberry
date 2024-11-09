@echo off

cd ..

cd vendor\raylib\

make -j

cd ..\..\

premake5 gmake2
make Raspberry

cd build\bin\Release\
start Raspberry.exe
