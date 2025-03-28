@echo off

cd ..

premake5 gmake2
make Raspberry config=debug_windows

cd build\bin\Debug\
start Raspberry.exe
