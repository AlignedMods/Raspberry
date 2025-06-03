@echo off

cd ..

premake5 gmake2
make Raspberry-Builder config=debug_windows

cd build\bin\Windows\Debug\
start Raspberry-Builder.exe
