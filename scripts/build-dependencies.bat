@echo off

cd ..

cd vendor\raylib\src\

make -j RAYLIB_RELEASE_PATH=../src
    
cd ..\..\..\
